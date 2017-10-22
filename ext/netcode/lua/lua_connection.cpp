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

static const struct luaL_Reg l_methods[] = {
    { "bind", connection_bind},
    { "set_callbacks", connection_set_callbacks},
    { NULL, NULL }
};

connection* connection_new(netcode* ctx, lua_State* L) {
    // 注册meta
    luaL_newmetatable(L, metatable_name);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, l_methods, 0);
    // end
    connection_userdata_t *data;

    // 使用newuserdata, 让lua 管理内存
    data = (connection_userdata_t*) lua_newuserdata(L, sizeof(connection_userdata_t));
    logw("new data: %p", data);
    data->ptr = ctx->create_connection(L);
    connaection* conn = data->ptr;
    conn->set_udata(data);
    luaL_getmetatable(L, metatable_name);
    lua_setmetatable(L, -2);

    return data->ptr;
}

static int connection_bind( lua_State *L ) {
    connection_userdata_t* data;
    data = (connection_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    int port = luaL_checkint(L, 2);
    if (data != NULL) {
        data->ptr->bind(port);
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
        conn->set_ref(callbacks_ref);
    }
    return 0;
}

