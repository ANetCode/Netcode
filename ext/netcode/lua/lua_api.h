#ifndef LUA_API_H
#define LUA_API_H
#include <lua.hpp>

struct lua_State;
class netcode;
class connection;
class message;

typedef struct {
    connection *ptr;
    int udata_ref;
} connection_userdata_t;

connection_userdata_t* connection_new(netcode* ctx, lua_State* L);

typedef struct {
    message *ptr;
    int udata_ref;
} message_userdata_t;

message_userdata_t* message_new(netcode* ctx, lua_State* L);

#endif //LUA_API_H