#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <lua.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<ctime>
#include <assert.h>

void l_push_table_string(lua_State* L , const char* key , const char* value);
void l_push_table_function(lua_State* L , const char* key , lua_CFunction value);

void l_invoke_table_function(lua_State* L, int ref, const char* func_name, int args);

void l_dump_stack(lua_State* L);

#endif // LUA_UTILS_H