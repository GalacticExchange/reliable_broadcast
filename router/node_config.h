#ifndef BROADCAST_NODE_CONFIG_H
#define BROADCAST_NODE_CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class NodeConfig {
    int id;
    std::string ip;
    int port;
    int localPort;
    std::string chainConfigDir;
    std::string pipesDir;

public:
    explicit NodeConfig(const std::string &configPath);

    boost::property_tree::ptree parseJson(std::string &confPath);

    int getId() const;

    const std::string &getIp() const;

    int getPort() const;

    int getLocalPort() const;

    const std::string &getChainDir() const;

    const std::string &getPipesDir() const;


private:
    void initFields(const boost::property_tree::ptree &json_config);

};

#endif //BROADCAST_NODE_CONFIG_H
