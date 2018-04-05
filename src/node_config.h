#ifndef BROADCAST_NODE_CONFIG_H
#define BROADCAST_NODE_CONFIG_H

#include "chain_config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>

#include <unordered_map>


class NodeConfig {
    int mId;
    std::string mIp;
    size_t mPort;
    std::string mChainConfigDir;
    std::string mPipesDir;
    std::string mRedisHost;
    size_t mRedisPort;
    mutable boost::shared_mutex mChainConfigsMutex;
    std::unordered_map<uint64_t, std::shared_ptr<ChainConfig>> mChainConfigs;

public:
    explicit NodeConfig(const std::string &configPath);

    boost::property_tree::ptree parseJson(std::string &confPath);
    int getId() const;
    const std::string &getIp() const;
    size_t getPort() const;
    int getLocalPort() const;
    const std::string &getChainDir() const;
    const std::string &getPipesDir() const;
    std::shared_ptr<const ChainConfig> getChainConfig(uint64_t chainHash) const;
    void setChainConfig(uint64_t chainHash, ChainConfig && chainConfig);
    void setChainConfig(uint64_t chainHash, const ChainConfig &chainConfig);
    std::string getRedisHost() const;
    size_t getRedisPort() const;
    void readChainConfigs();

private:
    void initFields(const boost::property_tree::ptree &json_config);
};

#endif //BROADCAST_NODE_CONFIG_H
