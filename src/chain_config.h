#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <unordered_map>


#include "node.h"


#ifndef BROADCAST_CHAIN_CONFIG_H
#define BROADCAST_CHAIN_CONFIG_H

#endif //BROADCAST_CHAIN_CONFIG_H


class ChainConfig {
    uint64_t mChainHash;
    int id;
    std::unordered_map<int, Node> nodes;
    std::string mChainPath;

public:

    explicit ChainConfig(std::string confPath);

    boost::property_tree::ptree parseJson(std::string &confPath);

    int getId() const;

    std::unordered_map<int, Node> getNodes() const;

    uint64_t getMChainHash() const;

    std::string getMChainPath() const;

private:

    void initFields(boost::property_tree::ptree json_config);

};