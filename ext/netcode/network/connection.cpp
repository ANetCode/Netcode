#include "connection.h"
#include "socket_utils.h"
#include "netcode.h"
#include "lua/lua_api.h"

#include <iostream>

#include <exception>
#include <typeinfo>
#include <stdexcept>

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

connection::connection(struct ev_loop* loop, lua_State* L) {
    m_loop = loop;
    this->L = L;
    callbacks_ref = 0;
    m_buffer.alloc(8*1024);

    // !!!! 这里会影响lua stack 创建message后需要pop 2
    message_userdata_t* msg_ud = message_new(netcode::get(), L);
    this->m_message = msg_ud->ptr;
    lua_pop(L, 2);
    //
}

connection::~connection() {
    delete m_message;
}

void connection::release() {
    invoke("on_disconnect", false);
    ::close(m_fd);
    m_fd = -1;
    ev_io_stop(m_loop, &m_io);
    delete this;
}

bool connection::bind(int port) {
    m_fd = SocketServer(nullptr, port);
    if (m_fd == -1) 
        return false;
    ev_io_init(&m_io, accept_cb, m_fd, EV_READ);
    m_io.data = this;
    ev_io_start(m_loop, &m_io);
    return true;
}

void connection::on_accept() {
    struct sockaddr_in   client_addr;
    socklen_t            len;
    int                  client_sd;
    struct ev_io        *w_client;
    int                  revents;

    if(EV_ERROR & m_io.events) { return; }
    // Accept client request
    client_sd = accept(m_io.fd, (struct sockaddr *)&client_addr, &len);

    if (client_sd < 0) { return; }

    // new connection
    connection_userdata_t* conn_udata = connection_new(netcode::get(), L);
    auto conn = conn_udata->ptr;
    conn->m_fd = client_sd;
    conn->m_io.data = conn;
    conn->m_udata = conn_udata;
    conn->callbacks_ref = this->callbacks_ref;
    ev_io_init(&conn->m_io, read_cb, client_sd, EV_READ);
    ev_io_start(m_loop, &conn->m_io);
    
    conn->invoke("on_connect", false);
    // l_invoke_table_function(L, callbacks_ref, "on_connect", m_fd);
}

void connection::on_read() {
    ssize_t bytes;
    int recv_size;

    if(EV_ERROR & m_io.events) {
        loge("got invalid event: %s", strerror(errno));
        return;
    }

    bytes = ::recv(m_fd, (char*)m_buffer.w_data(), m_buffer.remains(), 0);
    if (bytes == 0 || bytes == -1) { // read end or error
        logw("read body status: %zd", bytes);
        release();
        return;
    } else {
        m_buffer.set_size(m_buffer.size() + bytes);
        if (parse_message() == false) { // m_buffer 可能包含多条消息, 也可能包含单个消息切片
            release();
            return;
        }
    }
}

void connection::set_callbacks_ref(int ref) {
    this->callbacks_ref = ref;
}

int connection::get_fd() {
    return m_fd;
}

bool connection::parse_message() {
    // 没有达到可以解析的长度
    if (m_buffer.size() <= 4) {
        return false;
    }
    while(m_buffer.size() > 0) {
        if (m_message->parse(m_buffer) == false) {
            return false;// fmt error
        }
        if (m_message->is_ready()) {
            // logw("消息内容: %s", m_message->content().c_str());
            invoke("on_message", true);
            m_message->reset();
        }
    }
    return true;
}

void connection::invoke(const char* func_name, bool with_msg) {
    try {
        lua_rawgeti(L, LUA_REGISTRYINDEX, callbacks_ref);
        if (!lua_istable(L, -1)) {
            loge("引用错误 callbacks_ref: %d error. %s", callbacks_ref, luaL_typename(L, -1));
            l_dump_stack(L);
            return;
        }
        lua_getfield(L, -1, func_name);
        if (!lua_isfunction(L, -1)) {
            logd("function %s() not exits.\n", func_name);
            return;
        }
        // logd("invoke: %s", func_name);

        // args
        // 1 callbacks
        // 2 connection user data
        // 3 fd
        // 4 message //可选
        lua_insert(L, -2);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_udata->udata_ref);
        lua_pushinteger(L, m_fd);
        int n_args = 3;
        message_userdata_t* msg_udata = m_message->get_udata();
        if (with_msg && msg_udata!= nullptr) {
            lua_rawgeti(L, LUA_REGISTRYINDEX, msg_udata->udata_ref);
            n_args = 4;
        }

        int pcall_ret = lua_pcall(L, n_args, 0, 0); 

        if(pcall_ret != 0) {
            const char* errmsg = lua_tostring(L, -1);
            if(!errmsg) {
            errmsg = "(No Lua error message.)";
            logd("Unexpected Lua stack:\n");
            l_dump_stack (L);
            }
            logd("Lua error code %i: %s\n", pcall_ret, errmsg);
            lua_pop(L, 2);  // remove error and debug.traceback from the stack
            return;
        }
    } catch(const std::runtime_error& re) {
        loge("catch std::runtime_error: %s", re.what());
        release();
        return;
    } catch (const std::exception& ex) {
        loge("catch std::exception: %s", ex.what());
        release();
        return;
    } catch (const std::string& ex) {
        loge("catch exception: %s", ex.c_str());
        release();
        return;
    } catch (...) {
        loge("catch exception ... Possible memory corruption.");
        release();
        return;
    }
}

void connection::set_udata(connection_userdata_t* udata) {
    m_udata = udata;
}
void connection::send(std::string content) {
    content += "\0";
    int32_t len = content.size();
    buffer send_buf;
    send_buf.alloc(4 + content.size());
    send_buf.copy((uint8_t*)&len, 4);
    send_buf.copy((uint8_t*)content.c_str(), content.size());
    ::send(m_fd, send_buf.r_data(), send_buf.size(), 0);
}

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    connection* p = (connection*)watcher->data;
    if (p != NULL) {
        p->on_accept();
    }
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    connection* p = (connection*)watcher->data;
    if (p != NULL) {
        p->on_read();
    }
}
