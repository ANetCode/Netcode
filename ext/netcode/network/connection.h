#ifndef CONNECTION_H
#define CONNECTION_H
#include <ev.h>
#include <lua.hpp>
#include "common.h"
#include "lua/lua_api.h"
class connection {
public:
    connection(struct ev_loop*, lua_State*);
    void release();
    void bind(int port);
    
    void on_accept();
    void on_read();
    void set_ref(int ref);
    int  get_fd();
    void set_udata(connection_userdata_t*);
private:
    void invoke(const char* func_name);
private:
    lua_State             *L;
    int                    callbacks_ref;
    int                    m_fd;
    struct ev_io           m_io;
    struct ev_loop        *m_loop;
    buffer                 m_buffer;
    connection_userdata_t *m_udata;
};

#endif // CONNECTION_H