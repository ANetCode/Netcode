#include "Engine.h"
#include "Script.h"
#include "Actor.h"

Engine* Engine::GetInstance() {
    static Engine g_eng;
    return &g_eng;
}

Actor* Engine::CreateActor(std::string name) {
    // Actor* actor = Actor::Create();
    // return actor;
    return nullptr;
}

Engine::Engine() {
    SetTick(1);
}

void Engine::SetTick(int tickHZ) {
    this->tickHZ = tickHZ;
    timeDurationPerTick = (float)(1.0/tickHZ);
}

void Engine::Start() {
    isPause = false;
    isStop = false;
    Loop();
}

void Engine::Pause() {
    isPause = true;
}

void Engine::Stop() {
    isStop = true;
}


void Engine::Loop() {
    tickDeltaTime = 0;
    while(!isStop) {
        tickStartTime = clock();
        if (isPause) {
            WaitToNextTick();
            continue;
        }
        // add to start
        do {
            startList.clear();
            std::lock_guard<std::mutex> guard(mutexAddScript);
            for(Script* script : addedList) {
                startList.push_back(script);
            }
            addedList.clear();
        } while(0);

        // call start
        for(Script* script : startList) {
            if (script->IsRunnable()) {
                script->Start();
                if (!script->IsDestroyed()) {
                    updateList.push_back(script);
                } else {
                    destroyList.push_back(script);
                }
            } else {
                destroyList.push_back(script);
            }
        }
        startList.clear();
        // call update
        std::list<Script*> nextUpdateScripts;
        for(Script* script : updateList) {
            if (script->IsRunnable()) {
                script->Update();
                if (!script->IsDestroyed()) {
                    nextUpdateScripts.push_back(script);
                } else {
                    destroyList.push_back(script);
                }
            } else {
                destroyList.push_back(script);
            }
        }
        updateList.clear();
        updateList = nextUpdateScripts;
        // call destroy
        for(Script* script : destroyList) {
            script->Destroy();
            delete script;
        }
        destroyList.clear();

        WaitToNextTick();
    }
}

void Engine::WaitToNextTick() {
    unsigned int microseconds = 0;
    float t = CalcTickTimeLeft();
    if (t > 0) {
        microseconds = t * 1000 * 1000;
    } else {
        return;
    }

    usleep(microseconds);
    tickDeltaTime = (t + float(clock() - tickStartTime) / CLOCKS_PER_SEC) * 1000;
}

float Engine::CalcTickTimeLeft() {
    return (float) (timeDurationPerTick) - float(clock() - tickStartTime) / CLOCKS_PER_SEC;
}