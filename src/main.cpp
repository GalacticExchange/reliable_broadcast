#include "reliablebroadcast.h"

#include <iostream>
#include <unordered_map>

using namespace std;


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
    string chainConfigPath;

    if (argc >= 3) {
        nodeConfigPath = string(argv[1]);
        chainConfigPath = string(argv[2]);
    } else {
        nodeConfigPath = "node_config.json";
        chainConfigPath = "chain_config.json";
    }

    NodeConfig nodeConfig(nodeConfigPath);
    ChainConfig chainConfig(chainConfigPath);
    nodeConfig.setChainConfig(chainConfig.getMChainHash(), move(chainConfig));
    ReliableBroadcast(nodeConfig).start();

//    sender.join();

    return 0;
}
