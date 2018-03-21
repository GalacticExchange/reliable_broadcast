#include "chain_config.h"

using namespace std;

ChainConfig::ChainConfig(string confPath) {
    boost::property_tree::ptree json_config;
    json_config = parseJson(confPath);

    initFields(json_config);
}

boost::property_tree::ptree ChainConfig::parseJson(string &confPath) {
    boost::property_tree::ptree json_config;
    boost::property_tree::read_json(confPath, json_config);
    return json_config;
}

void ChainConfig::initFields(boost::property_tree::ptree json_config) {
    id = json_config.get<int>("id");
    cout << "ID: " << id << endl;
    mChainHash = json_config.get<uint64_t>("mChainHash");
    cout << "HASH: " << mChainHash << endl;
    mChainPath = json_config.get<string>("mChainPath");
    cout << "Path: " << mChainPath << endl;

    for (auto &item : json_config.get_child("comments")) { //todo "comments"?
        auto id = item.second.get<int>("id");
        string address = item.second.get<string>("host");
        auto port = item.second.get<int>("port");
        nodes[id] = Node(address, port);
    }
}

uint64_t ChainConfig::getMChainHash() const {
    return mChainHash;
}

int ChainConfig::getId() const {
    return id;
}

unordered_map<int, Node> ChainConfig::getNodes() const {
    return nodes;
}

std::string ChainConfig::getMChainPath() const {
    return mChainPath;
}
