#include <fcntl.h>

#include <iostream>
#include <unordered_map>

using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <capnp/message.h>
#include <capnp/serialize.h>

#include "../capnproto/skale_message.capnp.h"
#include "reliablebroadcast.h"


int main()
{
//    thread sender([](){
//        int fd = open("/tmp/m_chains/m_chain_5", O_WRONLY);

//        for (int i = 0;; ++i)
//        {
//            ::capnp::MallocMessageBuilder message;
//            SkaleMessage::Builder skaleMessageBuilder = message.initRoot<SkaleMessage>();
//            skaleMessageBuilder.setClientId(5);
//            skaleMessageBuilder.setMchainHash(555);
//            skaleMessageBuilder.setNonce(i);

//            capnp::byte buffer[5];
//            for (size_t j = 0; j < 5; ++j)
//            {
//                buffer[j] = 'a' + rand() % ('z' - 'a' + 1);
//            }
//            skaleMessageBuilder.setMessage(capnp::Data::Reader(buffer, 5));

//            capnp::writeMessageToFd(fd, message);

//            sleep(1);
//        }
//    });

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
