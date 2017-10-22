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
std::string l_print_by_type(lua_State* L, int idx);
std::string l_dump_table(lua_State* L, int idx);

std::string l_print_by_type(lua_State* L, int idx) {
    std::stringstream ss;
    ss << "\t";
    int type = lua_type(L, idx);
    switch(type) {
        case LUA_TSTRING: {
            ss << lua_tostring(L, idx);
        }break;
        case LUA_TBOOLEAN: {
            ss << lua_toboolean(L, idx);
        }break;
        case LUA_TNUMBER: {
            ss << lua_tonumber(L, idx);
        }break;
        case LUA_TUSERDATA: {
            ss << lua_touserdata(L, idx);
        }break;
        case LUA_TTABLE: {
            ss << "[table]";
        }break;
        case LUA_TLIGHTUSERDATA: {
            ss << lua_touserdata(L, idx) << " (light user data) ";
        }break;
        case LUA_TFUNCTION: {
            ss << lua_tocfunction(L, idx);
        }
        default: {
            ss << "[" << luaL_typename(L, idx) << "]";
        }
    }
    return ss.str();
}

std::string l_dump_table(lua_State* L, int idx) {
    std::stringstream ss;
    lua_pushvalue(L, idx);
    lua_pushnil(L);
    int i=0;
    while(lua_next(L, -2)) {
        lua_pushvalue(L, -2);
        // key
        int type = lua_type(L, -1);
        if (type == LUA_TSTRING) {
            ss << lua_tostring(L, -1);
        } else {
            ss << lua_tointeger(L, -1);
        }
        // VALUE
        ss << l_print_by_type(L, -2);
        //
        lua_pop(L, 2);
    }
    lua_pop(L, 1);
    return ss.str();
}

void l_dump_stack(lua_State* L) {
    std::stringstream ss;
    int nargs = lua_gettop(L);
    ss << "\ndump lua stack:\n";
    for (int i=1; i <= nargs; i++) {
        int idx = nargs - i;
        ss << idx + 1 << " => (" << luaL_typename(L, idx) << ")";
        ss << l_print_by_type(L, idx);
        ss << "\n";
    }
    logd("%s", ss.str().c_str());
}