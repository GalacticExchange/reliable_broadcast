#include "router.h"


using namespace std;

Router::Router(std::string nodeConfigPath) :
        nodeConfig(nodeConfigPath),
        outerSocket(nodeConfig.getPort()),
        innerSocket(outerSocket, mChains, UDP_INNER_PORT) {

//    signal(SIGPOLL, this->pollHandler);
    readChainConfigs(nodeConfig.getMChainDirPath());
}

OuterSocket &Router::getOuterSocket() {
    return outerSocket;
}

void Router::addMChain(ChainConfig &config) {
    vector<Node> nodes;
    nodes.reserve(config.getNodes().size());

    for (auto kv : config.getNodes()) {
        nodes.push_back(kv.second);
    }

    mChains[config.getMChainHash()] = nodes;
}

/***
 * Blocking call
 */
void Router::start() {
    outerThread = std::thread([this]() {
        outerSocket.listen();
    });
    innerThread = std::thread([this]() {
        innerSocket.listen();
    });

    outerThread.join();
    innerThread.join();
}

void Router::readChainConfigs(const string &configsDir) {
    vector<string> configs = FileUtils::listFiles(configsDir);
    for (const string &configPath : configs) {
        ChainConfig chainConfig(configPath);
        addMChain(chainConfig);
    }
}

//void Router::pollHandler(int signum) {
//    cout << "Signal (" << signum << ") received.\n";
//    //todo poll new mChains
//}
