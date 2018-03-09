#include <iostream>
#include <unordered_map>

using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "reliablebroadcast.h"


int main()
{
    srand(clock());

    boost::property_tree::ptree json_config;
    boost::property_tree::read_json("config.json", json_config);

    int id = json_config.get<int>("id");

    unordered_map<int, Node> nodes;
    for (auto& item : json_config.get_child("nodes"))
    {
        int id = item.second.get<int>("id");
        string address = item.second.get<string>("host");
        int port = item.second.get<int>("port");
        nodes[id] = Node(address, port);
    }

    ReliableBroadcast(id, nodes).start();

    return 0;
}
