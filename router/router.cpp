#include "router.h"


using namespace std;

Router::Router(std::string nodeConfigPath) :
        nodeConfig(nodeConfigPath),
        outerSocket(nodeConfig.getPort(), const_cast<string &>(nodeConfig.getPipesDir())),
        innerSocket(outerSocket, mChains, nodeConfig.getLocalPort())
{

//    signal(SIGPOLL, this->pollHandler);
    readChainConfigs(nodeConfig.getChainDir());    
}

OuterSocket &Router::getOuterSocket() {
    return outerSocket;
}

//InnerSocket &Router::getInnerSocket() {
//    return innerSocket;
//}

void Router::addMChain(ChainConfig &config) {
    vector<Node> nodes;
    nodes.reserve(config.getNodes().size() - 1);

    for (auto kv : config.getNodes()) {
        if (kv.second.getId() != nodeConfig.getId()){
            nodes.push_back(kv.second);
        }
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

//    innerThread = std::thread([this]() {
//        innerSocket.listen();
//    });
//    mInnerSocket.startListen(nodeConfig.getLocalPort(), [](shared_ptr<vector<char>> buffer)
//    {
//        cerr << "Received " << buffer->size() << " bites" << endl;
//    });

    outerThread.join();
//    innerThread.join();
}

void Router::readChainConfigs(const string &configsDir) {
    vector<string> configs = FileUtils::listFiles(configsDir);
    for (const string &configPath : configs) {
        ChainConfig chainConfig(configPath);
        addMChain(chainConfig);
    }
}

NodeConfig &Router::getNodeConfig() {
    return nodeConfig;
}

//void Router::pollHandler(int signum) {
//    cout << "Signal (" << signum << ") received.\n";
//    //todo poll new mChains
//}
