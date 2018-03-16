#include <fcntl.h>

#include <iostream>
#include <unordered_map>

using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "reliablebroadcast.h"


int main()
{
    thread sender([](){
        ofstream os("/tmp/m_chains/m_chain_5", ios::binary);

        for (int i = 0;; ++i)
        {
            Message message(i, i, i, i, Message::MessageType::SEND, vector<char>());
            vector<char> buffer = message.encode();

            uint16_t length = buffer.size();
            os.write(reinterpret_cast<char*>(&length), sizeof(length));
            os.write(&*buffer.begin(), buffer.size());
            os.flush();

            sleep(1);
        }
    });

    srand(clock());

    boost::property_tree::ptree json_config;
    boost::property_tree::read_json("config.json", json_config);

    int id = json_config.get<int>("id");

    uint64_t mChainHash = json_config.get<uint64_t>("mChainHash");

    unordered_map<int, Node> nodes;
    for (auto& item : json_config.get_child("nodes"))
    {
        int id = item.second.get<int>("id");
        string address = item.second.get<string>("host");
        int port = item.second.get<int>("port");
        nodes[id] = Node(address, port);
    }

    ReliableBroadcast(id, mChainHash, nodes).start();

//    sender.join();

    return 0;
}
