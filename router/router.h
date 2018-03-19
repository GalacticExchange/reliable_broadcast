#include "outer_socket.h"
#include "../src/chain_config.h"

#ifndef BROADCAST_ROUTER_H
#define BROADCAST_ROUTER_H

#endif //BROADCAST_ROUTER_H



class Router{
    OuterSocket controller;
    const int UDP_PORT = 1234;
public:
    explicit Router(ChainConfig config);


};


