#ifndef NETCODE_H
#define NETCODE_H

#include "common.h"
#include "lua/lua_utils.h"
#include <chrono>
#include <string>
#include <ev++.h>
#include "network/connection.h"


class netcode {
public:
    netcode();
    static netcode* get() {
        return g_netcode;
    }
    void add(int amount);
    float elapsed_time();
    void start();
    void update();
    void send(std::string);

    bool is_running();

    connection* create_connection(lua_State*);
    void release_connection(connection*);
private:
    int amount;
    std::chrono::time_point<std::chrono::system_clock> start_time;

    bool            b_running;
    struct ev_loop *loop;
    static netcode *g_netcode; 
};

#endif