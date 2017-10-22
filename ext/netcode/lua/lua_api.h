#ifndef LUA_API_H
#define LUA_API_H
#include <lua.hpp>

struct lua_State;
class netcode;
class connection;

connection* connection_new(netcode* ctx, lua_State* L);

typedef struct {
    connection *ptr;
    int ref;
    int ref_on_connect;
} connection_userdata_t;

#endif //LUA_API_H