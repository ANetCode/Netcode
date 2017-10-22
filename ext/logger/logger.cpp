#include <iostream>
int printlib() {
    std::cout << "hello printlib" << std::endl;
    return 0;
} 
#include <lua.hpp>
static int l_debug( lua_State *L )
{
   puts("l_debug called.");
   return 0;
}
extern "C" {

int luaopen_ext_logger( lua_State *L )  {
   static const struct luaL_Reg libs[] = {
      { "debug", l_debug },
      { NULL, NULL }
   }; 
   luaL_newlib( L, libs );   // create table containing `bar`
   lua_setglobal(L, "logger"); // assign that table to global `foo`
   return 1;
}

} // end extern C