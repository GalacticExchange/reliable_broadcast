#include "router.h"
#include <thread>

using namespace std;

Router::Router() :
        outerSocket(UDP_OUTER_PORT),
        innerSocket(outerSocket, mChains, UDP_INNER_PORT) {

//    signal(SIGPOLL, this->pollHandler);
}

OuterSocket & Router::getOuterSocket(){
    return outerSocket;
}

void Router::addMChain(ChainConfig &config) {
    vector<Node> nodes;
    nodes.reserve(config.getNodes().size());

    for (auto kv : config.getNodes()) {
        nodes.push_back(kv.second);
    }

    mChains[config.getId()] = nodes;
}

/***
 * Blocking call
 */
void Router::start() {
    outerThread = std::thread([this](){
       outerSocket.listen();
    });
    innerThread = std::thread([this](){
        innerSocket.listen();
    });

    outerThread.join();
    innerThread.join();
}

//void Router::pollHandler(int signum) {
//    cout << "Signal (" << signum << ") received.\n";
//    //todo poll new mChains
//}
