#include "connection.h"
#include "socket_utils.h"
#include "netcode.h"
#include "lua/lua_api.h"

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

connection::connection(struct ev_loop* loop, lua_State* L) {
    m_loop = loop;
    this->L = L;
    callbacks_ref = 0;
    m_buffer.alloc(8*1024);
}

void connection::release() {
    // l_invoke_table_function(L, callbacks_ref, "on_disconnect", m_fd);
    invoke("on_disconnect");
    ::close(m_fd);
    m_fd = -1;
    ev_io_stop(m_loop, &m_io);
    // m_buffer.release();
}

void connection::bind(int port) {
    int fd = SocketServer(nullptr, port);
    ev_io_init(&m_io, accept_cb, fd, EV_READ);
    m_io.data = this;
    ev_io_start(m_loop, &m_io);
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
    auto conn = connection_new(netcode::get(), L);
    conn->m_fd = client_sd;
    conn->m_io.data = conn;
    ev_io_init(&conn->m_io, read_cb, client_sd, EV_READ);
    ev_io_start(m_loop, &conn->m_io);
    logw("新连接: %p", conn);
    l_dump_stack(L);
    invoke("on_connect");
    // l_invoke_table_function(L, callbacks_ref, "on_connect", m_fd);
}

void connection::on_read() {
    // int BUFFER_SIZE = 8 * 1024;
    // char buffer[BUFFER_SIZE];
    ssize_t bytes;
    int recv_size;

    if(EV_ERROR & m_io.events) {
        loge("got invalid event: %s", strerror(errno));
        return;
    }
    // calc recv
    bytes = ::recv(m_fd, (char*)m_buffer.w_data(), m_buffer.remains(), 0);
    if (bytes == 0 || bytes == -1) { // read end or error
        logw("read status: %zd", bytes);
        release();
        return;
    } else {
        logd("buffer: %s", m_buffer.r_data());
        m_buffer.reset();
    }
}

void connection::set_ref(int ref) {
    this->callbacks_ref = ref;
}

int connection::get_fd() {
    return m_fd;
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

void connection::invoke(const char* func_name) {
    int ref = callbacks_ref;
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_istable(L, -1)) {
        loge("引用错误 ref: %d error. %s", ref, luaL_typename(L, -1));
        l_dump_stack(L);
        return;
    }
    lua_getfield(L, -1, func_name);
    if (!lua_isfunction(L, -1)) {
        logd("function %s() not exits.\n", func_name);
        return;
    }
    logd("invoke: %s", func_name);
    l_dump_stack(L);
    
    lua_insert(L, -2);
    // args
    // lua_pushlightuserdata(L, m_udata);
    lua_pushinteger(L, m_fd);

    l_dump_stack(L);
    int pcall_ret = lua_pcall(L, 2, 0, 0); 

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
}

void connection::set_udata(connection_userdata_t* udata) {
    m_udata = udata;
}