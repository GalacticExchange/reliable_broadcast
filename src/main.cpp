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
//    thread sender([](){
//        ofstream os("/tmp/m_chains/m_chain_5", ios::binary);

//        for (int i = 0;; ++i)
//        {
//            Message message(i, i, 5, i, Message::MessageType::SEND, vector<char>());
//            vector<char> buffer = message.encode();

//            uint16_t length = buffer.size();
//            os.write(reinterpret_cast<char*>(&length), sizeof(length));
//            os.write(&*buffer.begin(), buffer.size());
//            os.flush();

//            sleep(1);
//        }
//    });

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

//    sender.join();

    return 0;
}
