#include "netcode.h"
#include <thread>

netcode* netcode::g_netcode = nullptr;

netcode::netcode() {
    assert(g_netcode == nullptr);
    g_netcode = this;
    
    b_running = false;
}

void netcode::add(int amount) {
    this->amount += amount;
}

float netcode::elapsed_time() {
    std::chrono::duration<float> duration = (std::chrono::system_clock::now() - start_time);
    return duration.count();
}

void netcode::start() {
    if (b_running == true) return;
    start_time = std::chrono::system_clock::now();
    loop = ev_loop_new();
    logd("netcode start.");
    b_running = true;
    // new std::thread([](netcode *ptr){
    //     while (ptr->is_running()) {
    //         ptr->update();
    //     }
    // }, this);
}
void netcode::update() {
    if (b_running) {
        ev_loop(loop, EVLOOP_NONBLOCK);
    }
}
void netcode::send(std::string) {}

connection* netcode::create_connection(lua_State* L){
    return new connection(loop, L);
}

void netcode::release_connection(connection* c) {
    c->release();
    delete c;
}

bool netcode::is_running() {
    return b_running;
}