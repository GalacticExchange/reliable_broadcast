#ifndef BROADCAST_CHAIN_CONFIG_H
#define BROADCAST_CHAIN_CONFIG_H

#include "node.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <unordered_map>


class ChainConfig {
    uint64_t mChainHash;
    std::unordered_map<int, Node> nodes;
    std::string mChainPath;

public:

    explicit ChainConfig(const std::string &confPath);

    boost::property_tree::ptree parseJson(const std::string &confPath);

    std::unordered_map<int, Node> getNodes() const;

    uint64_t getMChainHash() const;

    std::string getMChainPath() const;

private:

    void initFields(boost::property_tree::ptree json_config);

};

#endif //BROADCAST_CHAIN_CONFIG_H
