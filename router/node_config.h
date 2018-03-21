#ifndef BROADCAST_NODE_CONFIG_H
#define BROADCAST_NODE_CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


class NodeConfig {
    int id;
    std::string ip;
    int port;
    std::string mChainDirPath;

public:
    explicit NodeConfig(const std::string &configPath);

    boost::property_tree::ptree parseJson(std::string &confPath);

    int getId() const;

    const std::string &getIp() const;

    int getPort() const;

    const std::string &getMChainDirPath() const;


private:
    void initFields(const boost::property_tree::ptree &json_config);

};

#endif //BROADCAST_NODE_CONFIG_H
