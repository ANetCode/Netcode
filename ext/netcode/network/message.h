#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"
#include "lua/lua_api.h"

class message {
public:
    message(lua_State* L);
    bool parse(buffer& buffer);
    void reset();
    bool is_ready();
    std::string content();
    void set_udata(message_userdata_t* ud) { m_udata = ud; }
    message_userdata_t* get_udata() { return m_udata; }
private:
    bool        m_ready;
    bool        m_message_start;
    buffer      m_buffer;
    size_t      m_length;
    std::string m_content;
    lua_State  *L;
    message_userdata_t *m_udata;
};

#endif // MESSAGE_H