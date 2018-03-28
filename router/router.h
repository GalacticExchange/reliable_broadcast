#ifndef BROADCAST_ROUTER_H
#define BROADCAST_ROUTER_H

#include "../src/chain_config.h"
#include "inner_socket.h"
#include "node_config.h"
#include "outer_socket.h"
#include "socket.h"

#include <thread>


class Router {
    NodeConfig nodeConfig;
    OuterSocket outerSocket;
//    Socket mOuterSocket;

    InnerSocket innerSocket;
//    Socket mInnerSocket;
    std::unordered_map<int, std::vector<Node>> mChains;

    std::thread innerThread;
    std::thread outerThread;

public:
    explicit Router(std::string nodeConfigPath);

    void addMChain(ChainConfig &config);

    void start();

    OuterSocket &getOuterSocket();
//    InnerSocket &getInnerSocket();

    void readChainConfigs(const std::string &configsDir);

    NodeConfig &getNodeConfig();

//// todo parse from config
//    static const int UDP_OUTER_PORT = 1234;
//    static const int UDP_INNER_PORT = 1122;

private:
//    void pollHandler(int signum);
};


#endif //BROADCAST_ROUTER_H





