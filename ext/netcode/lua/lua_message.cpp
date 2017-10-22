#include "network/connection.h"
#include "lua_api.h"
#include "netcode.h"
#include "network/message.h"

static const char* metatable_name = "Lmessage";

static message* get_instance(lua_State* L) {
    message_userdata_t* data = (message_userdata_t*) luaL_checkudata(L, 1, metatable_name);
    if (data != NULL) {
        return data->ptr;
    }
    return nullptr;
} 

static int message_content( lua_State *L );

static const struct luaL_Reg l_methods[] = {
    { "content", message_content},
    { NULL, NULL }
};

message_userdata_t* message_new(netcode* ctx, lua_State* L) {
    // 注册meta
    luaL_newmetatable(L, metatable_name);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, l_methods, 0);
    // end

    // 使用newuserdata, 让lua 管理内存
    message_userdata_t *data;
    data = (message_userdata_t*) lua_newuserdata(L, sizeof(message_userdata_t));
    data->udata_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    lua_rawgeti(L, LUA_REGISTRYINDEX, data->udata_ref);
    
    data->ptr = new message(L);
    auto msg = data->ptr;
    msg->set_udata(data);
    
    luaL_getmetatable(L, metatable_name);
    lua_setmetatable(L, -2);
    
    return data;
}

static int message_content( lua_State *L ) {
    auto* ptr = get_instance(L);
    if (ptr != nullptr) {
        lua_pushstring(L, ptr->content().c_str());
        return 1;
    }
    return 0;
}
