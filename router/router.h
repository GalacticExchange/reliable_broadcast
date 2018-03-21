

#ifndef BROADCAST_ROUTER_H
#define BROADCAST_ROUTER_H

#include "outer_socket.h"
#include "inner_socket.h"
#include "node_config.h"
#include <thread>


#include "../src/chain_config.h"

class Router {
    NodeConfig nodeConfig;
    OuterSocket outerSocket;
    InnerSocket innerSocket;
    std::unordered_map<int, std::vector<Node>> mChains;

    std::thread innerThread;
    std::thread outerThread;

    const int UDP_INNER_PORT = 1122;
public:
    explicit Router(std::string nodeConfigPath);

    void addMChain(ChainConfig &config);

    void start();

    OuterSocket &getOuterSocket();

    void readChainConfigs(const std::string &configsDir);

// todo parse from config
    static const int UDP_OUTER_PORT = 1234;
private:
//    void pollHandler(int signum);
};


#endif //BROADCAST_ROUTER_H





