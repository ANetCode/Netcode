#ifndef NC_ENGINE_H
#define NC_ENGINE_H

#include "netcode.h"

class Engine {
public:
    static Engine* GetInstance();
    Actor* CreateActor(std::string name);
    void SetTick(int tickHZ);
    void Start();
    void Pause();
    void Stop();
protected:
    Engine();
private:
    void Loop();
    void WaitToNextTick();
    float CalcTickTimeLeft();
protected:
    int tickHZ;
    bool isPause;
    bool isStop;
    float timeDurationPerTick;
    float tickStartTime;
    float tickDeltaTime;

    std::list<Script*> startList;
    std::list<Script*> updateList;
    std::list<Script*> destroyList;

    std::mutex mutexAddScript;
    std::list<Script*> addedList;
    std::unordered_map<std::string, Actor*> actorMap;
    // std::unordered_map<std::string, std::list<ScriptEvent>> scriptEventMap;
};

#endif // NC_ENGINE_H