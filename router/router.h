

#ifndef BROADCAST_ROUTER_H
#define BROADCAST_ROUTER_H

#include "outer_socket.h"
#include "inner_socket.h"
#include <thread>


#include "../src/chain_config.h"

class Router {
    OuterSocket outerSocket;
    InnerSocket innerSocket;
    std::unordered_map<int, std::vector<Node>> mChains;

    std::thread innerThread;
    std::thread outerThread;

    const int UDP_INNER_PORT = 1122;
public:
    Router();

    void addMChain(ChainConfig &config);

    void start();

    OuterSocket &getOuterSocket();

// todo parse from config
    static const int UDP_OUTER_PORT = 1234;
private:
//    void pollHandler(int signum);
};


#endif //BROADCAST_ROUTER_H





