#include "chain_config.h"

using namespace std;

ChainConfig::ChainConfig(const string &confPath) {
    boost::property_tree::ptree json_config;
    json_config = parseJson(confPath);

    initFields(json_config);
}

boost::property_tree::ptree ChainConfig::parseJson(const string &confPath) {
    boost::property_tree::ptree json_config;
    boost::property_tree::read_json(confPath, json_config);
    return json_config;
}

void ChainConfig::initFields(boost::property_tree::ptree json_config) {
    mChainHash = json_config.get<uint64_t>("mChainHash");
    for (auto &item : json_config.get_child("nodes")) {
        int id = item.second.get<int>("id");
        string address = item.second.get<string>("ip");
        auto port = item.second.get<int>("port");
        Node node(id, port, address);
        nodes.insert(std::make_pair(id,node));
    }
}

uint64_t ChainConfig::getMChainHash() const {
    return mChainHash;
}

const unordered_map<int, Node> &ChainConfig::getNodes() const {
    return nodes;
}

//std::string ChainConfig::getMChainPath() const {
//    return mChainPath;
//}
