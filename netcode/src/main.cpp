#include <iostream>
using namespace std;

// #include "netcode.h"
#include <lua.hpp>

int main(int argc, char const *argv[]) {
    // cout << "Hello Netcode" << endl;
    if (argc != 2) {
        return -1;
    }

    const char* filename = argv[1];
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, filename);
    // Actor* actor = Engine::GetInstance()->CreateActor("master");

    // Engine::GetInstance()->Start();
    return 0;
}