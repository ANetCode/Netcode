#ifndef NC_ACTOR_H
#define NC_ACTOR_H

#include "netcode.h"

class Actor {
protected:
    Actor();
public:
    virtual ~Actor() {}
    Script* Load(std::string path);
    void SetName(std::string name);
    lua_State* GetLua() { return L; }
protected:
    std::string name;
    lua_State* L;
};

#endif // NC_ACTOR_H