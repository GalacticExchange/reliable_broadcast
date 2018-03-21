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

    chainConfigDir = json_config.get<string>("chainConfigDir");
    cout << "Node config chainConfigDir: " << chainConfigDir << endl;

    pipesDir = json_config.get<string>("pipesDir");
    cout << "Node config pipesDir: " << pipesDir << endl;


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

const string &NodeConfig::getChainDir() const {
    return chainConfigDir;
}

const string &NodeConfig::getPipesDir() const {
    return pipesDir;
}
