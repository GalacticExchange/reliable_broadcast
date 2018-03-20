#include <iostream>
#include <unordered_map>

using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "reliablebroadcast.h"


int main()
{
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

    ChainConfig config("config.json");
    ReliableBroadcast(config).start();

//    sender.join();

    return 0;
}
