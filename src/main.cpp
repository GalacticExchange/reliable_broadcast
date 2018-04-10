#include "reliablebroadcast.h"

#include <boost/log/trivial.hpp>

#include <iostream>
#include <unordered_map>

using namespace std;


NodeConfig *config = nullptr;

void configUpdateSignalHandler(int)
{
    if (config)
    {
        BOOST_LOG_TRIVIAL(info) << "Reload chain configs";
        config->readChainConfigs();
    }
}

int main(int argc, char *argv[]) {
    srand(clock());

    string nodeConfigPath;    

    if (argc >= 2) {
        nodeConfigPath = string(argv[1]);        
    } else {
        nodeConfigPath = "node_config.json";
    }

    NodeConfig nodeConfig(nodeConfigPath);
    config = &nodeConfig;
    signal(SIGHUP, configUpdateSignalHandler);

    ReliableBroadcast(nodeConfig).start();

    return 0;
}
