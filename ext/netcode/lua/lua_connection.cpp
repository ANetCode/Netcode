#include "network/connection.h"
#include <chrono>
#include <thread>
#include "lua_api.h"
#include "netcode.h"

static const char* metatable_name = "Lconnection";

static connection* get_instance(lua_State* L) {
    connection_userdata_t* data = (connection_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (data != NULL) {
        return data->ptr;
    }
    return nullptr;
} 

static int connection_bind( lua_State *L );
static int connection_set_callbacks( lua_State *L );
static int connection_echo( lua_State *L );
static int connection_send( lua_State *L );

static const struct luaL_Reg l_methods[] = {
    { "bind", connection_bind},
    { "set_callbacks", connection_set_callbacks},
    { "echo", connection_echo},
    { "send", connection_send},
    { NULL, NULL }
};

connection_userdata_t* connection_new(netcode* ctx, lua_State* L) {
    // 注册meta
    luaL_newmetatable(L, metatable_name);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, l_methods, 0);
    // end

    // 使用newuserdata, 让lua 管理内存
    connection_userdata_t *data;
    data = (connection_userdata_t*) lua_newuserdata(L, sizeof(connection_userdata_t));
    data->udata_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_rawgeti(L, LUA_REGISTRYINDEX, data->udata_ref);

    data->ptr = ctx->create_connection(L);
    connection* conn = data->ptr;
    conn->set_udata(data);
    luaL_getmetatable(L, metatable_name);
    lua_setmetatable(L, -2);
    
    return data;
}

static int connection_bind( lua_State *L ) {
    connection* conn = get_instance(L);
    int port = luaL_checkint(L, 2);
    if (conn) {
        if (conn->bind(port) == false)
            return 0;
        return 1;
    }
    return 0;
}

static int connection_set_callbacks( lua_State *L ) {
    connection* conn = get_instance(L);

    lua_insert(L, -2);
    lua_settop(L, 1);
    if (lua_istable(L, 1) && conn != nullptr) {
        int callbacks_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        conn->set_callbacks_ref(callbacks_ref);
    }
    return 0;
}

static int connection_echo( lua_State *L ) {
    connection* conn = get_instance(L);
    loge("hello echo!");
    return 0;
}

static int connection_send( lua_State *L ) {
    connection* conn = get_instance(L);
    if (!lua_isstring(L, 2)) {
        return 0;
    }
    const char* content = luaL_checkstring(L, 2);
    conn->send(content);
    return 0;
}
