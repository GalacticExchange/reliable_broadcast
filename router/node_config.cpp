#include "node_config.h"

using namespace std;

NodeConfig::NodeConfig(const std::string &configPath) {
    boost::property_tree::ptree json_config;
    json_config = parseJson(const_cast<string &>(configPath));

    initFields(json_config);
}

boost::property_tree::ptree NodeConfig::parseJson(std::string &confPath) {
    boost::property_tree::ptree json_config;
    boost::property_tree::read_json(confPath, json_config);
    return json_config;
}

void NodeConfig::initFields(const boost::property_tree::ptree &json_config) {
    id = json_config.get<int>("id");
    cout << "Node config ID: " << id << endl;

    ip = json_config.get<string>("ip");
    cout << "Node config IP: " << ip << endl;

    port = json_config.get<int>("port");
    cout << "Node config PORT: " << port << endl;

    chainConfigDirPath = json_config.get<string>("chainConfigDirPath");
    cout << "Node config chainConfigDirPath: " << chainConfigDirPath << endl;

}

int NodeConfig::getId() const {
    return id;
}

const string &NodeConfig::getIp() const {
    return ip;
}

int NodeConfig::getPort() const {
    return port;
}

const string &NodeConfig::getMChainDirPath() const {
    return chainConfigDirPath;
}
