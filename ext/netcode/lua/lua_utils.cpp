#include "lua_utils.h"
#include <sstream>
#include "common.h"

void l_push_table_string(lua_State* L , const char* key , const char* value) {
    lua_pushstring(L, key);
    lua_pushstring(L, value);
    lua_settable(L, -3);
} 

void l_push_table_function(lua_State* L , const char* key , lua_CFunction value) {
    lua_pushstring(L, key);
    lua_pushcfunction(L, value);
    lua_settable(L, -3);
}

void l_invoke_table_function(lua_State* L, int ref, const char* func_name, int args) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    if (!lua_istable(L, -1)) {
        loge("ref: %d error. %s", ref, luaL_typename(L, 1));
        l_dump_stack(L);
        return;
    }
    lua_getfield(L, -1, func_name);
    if (!lua_isfunction(L, -1)) {
        logd("function %s() not exits.\n", func_name);
        return;
    }
    logd("invoke: %s", func_name);
    lua_insert(L, -2);
    // args
    lua_pushinteger(L, args);
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

void l_dump_stack(lua_State* L) {
    std::stringstream ss;
    int nargs = lua_gettop(L);
    ss << "\n";
    for (int i=1; i <= nargs; i++) {
        int idx = nargs - i;
        int type = lua_type(L, idx);
        const char* type_name = lua_typename(L, type);
        ss << idx + 1 << " => (" << type_name << ")";
        if (type == LUA_TSTRING) {
            ss << " = " << lua_tostring(L, idx);
        } else if (type == LUA_TUSERDATA){
            ss << " = " << lua_touserdata(L, idx);
        }
        ss << "\n";
    }
    logd("%s", ss.str().c_str());
}