#ifndef CONNECTION_H
#define CONNECTION_H
#include <ev.h>
#include <lua.hpp>
#include "common.h"
#include "message.h"
#include "lua/lua_api.h"

class connection {
public:
    connection(struct ev_loop*, lua_State*);
    virtual ~connection();
    void release();
    bool bind(int port);
    
    void on_accept();
    void on_read();

    void send(std::string content);

    void set_callbacks_ref(int ref);
    void set_ref(int ref);
    int  get_fd();
    void set_udata(connection_userdata_t*);
private:
    void invoke(const char* func_name, bool with_msg);
    bool parse_message();
private:
    lua_State             *L;
    int                    callbacks_ref;
    int                    m_fd;
    struct ev_io           m_io;
    struct ev_loop        *m_loop;
    buffer                 m_buffer;
    connection_userdata_t *m_udata;
    message               *m_message;
};

#endif // CONNECTION_H