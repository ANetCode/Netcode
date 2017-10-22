#include "netcode.h"
#include <chrono>
#include <thread>
#include "lua_api.h"
static const char* metatable_name = "Lnetcode";
typedef struct {
    netcode    *ptr;
} netcode_userdata_t;

static netcode* get_instance( lua_State *L ) {
    netcode_userdata_t* data = (netcode_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (data) {
        return data->ptr;
    }
    return nullptr;
}

static int netcode_new(lua_State* L) {
    netcode_userdata_t *data;
    const char        *name;
    name = luaL_checkstring(L, 1);
    if (name == NULL) {
        luaL_error(L, "namec can't be empty");
        return 0;
    }
    // 使用newuserdata, 让lua 管理内存
    data = (netcode_userdata_t*) lua_newuserdata(L, sizeof(netcode_userdata_t));
    data->ptr = nullptr;

    data->ptr = new netcode();

    luaL_getmetatable(L, metatable_name);
    lua_setmetatable(L, -2);

    return 1;
}

static int netcode_sleep( lua_State *L ) {
    if(lua_isnumber(L, -1)) {
        long duration_ms = lua_tointeger(L, -1);
        // usleep(duration_ms*1000);
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms));
    }
    return 0;
}

static int netcode_add( lua_State *L ) {
    netcode_userdata_t* nc;
    int                 amount;
    nc = (netcode_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    amount = luaL_checkint(L, 2);
    if (nc != NULL) {
        nc->ptr->add(amount);
    }
    return 0;
}

static int netcode_time( lua_State *L ) {
    netcode_userdata_t* nc;
    nc = (netcode_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (nc != NULL) {
        lua_pushnumber(L, nc->ptr->elapsed_time());
        return 1;
    }
    return 0;
}

static int netcode_start( lua_State *L ) {
    netcode_userdata_t* data;
    printf("invoke start");
    data = (netcode_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (data != NULL) {
        data->ptr->start();
        return 1;
    }
    return 0;
}

static int netcode_update( lua_State *L ) {
    netcode* nc = get_instance(L);
    if (nc) {
        nc->update();
    }
    return 0;
}
static int netcode_create_connection( lua_State *L ) {
    netcode_userdata_t* nc;
    nc = (netcode_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (nc != NULL) {
        connection_new(nc->ptr, L);
        return 1;
    }
    return 0;
}

static int netcode_release_connection( lua_State *L ) {
    
    return 0;
}

static const struct luaL_Reg nc_methods[] = {
    { "sleep", netcode_sleep},
    { "add",   netcode_add},
    { "time",  netcode_time},
    { "start", netcode_start},
    { "update",  netcode_update},
    { "create_connection",  netcode_create_connection},
    { "release_connection", netcode_release_connection},
    { NULL, NULL }
};

static const struct luaL_Reg l_funcs[] = {
    { "new", netcode_new },
    { NULL, NULL }
};

extern "C" {

int luaopen_ext_netcode( lua_State *L ) {
    luaL_newmetatable(L, metatable_name);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, nc_methods, 0);

    luaL_newlib(L, l_funcs); 
    return 1;
}
}