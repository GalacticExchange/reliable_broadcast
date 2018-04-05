#include "../router/file_utils.h"
#include "node_config.h"

#include <boost/log/trivial.hpp>

using std::make_pair;
using std::move;
using std::string;
using std::vector;


std::string NodeConfig::getRedisHost() const
{
    return mRedisHost;
}

size_t NodeConfig::getRedisPort() const
{
    return mRedisPort;
}

NodeConfig::NodeConfig(const string &configPath) {
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
    mId = json_config.get<int>("id");
    BOOST_LOG_TRIVIAL(info) << "Node config ID: " << mId;

    mIp = json_config.get<string>("ip");
    BOOST_LOG_TRIVIAL(info) << "Node config IP: " << mIp;

    mPort = json_config.get<size_t>("port");
    BOOST_LOG_TRIVIAL(info) << "Node config PORT: " << mPort;

    mChainConfigDir = json_config.get<string>("chainConfigDir");
    BOOST_LOG_TRIVIAL(info) << "Node config chainConfigDir: " << mChainConfigDir;

    mPipesDir = json_config.get<string>("pipesDir");
    BOOST_LOG_TRIVIAL(info) << "Node config pipesDir: " << mPipesDir;

    mRedisHost = json_config.get<string>("redisHost");
    BOOST_LOG_TRIVIAL(info) << "Node config redisHost: " << mRedisHost;

    mRedisPort = json_config.get<size_t>("redisPort");
    BOOST_LOG_TRIVIAL(info) << "Node config redisPort: " << mRedisPort;

    readChainConfigs(mChainConfigDir);
    std::cerr << std::endl;
}

void NodeConfig::readChainConfigs(const string &configsDir)
{
    vector<string> configs = FileUtils::listFiles(configsDir);
    for (const string &configPath : configs) {
        ChainConfig chainConfig(configPath);
        setChainConfig(chainConfig.getMChainHash(), move(chainConfig));
    }
}

int NodeConfig::getId() const {
    return mId;
}

const string &NodeConfig::getIp() const {
    return mIp;
}

size_t NodeConfig::getPort() const {
    return mPort;
}

const string &NodeConfig::getChainDir() const {
    return mChainConfigDir;
}

const string &NodeConfig::getPipesDir() const {
    return mPipesDir;
}

const ChainConfig &NodeConfig::getChainConfig(uint64_t chainHash) const
{
    auto config_ptr = mChainConfigs.find(chainHash);
    if (config_ptr == mChainConfigs.end())
    {
        throw std::logic_error("Request config of non existing chain");
    }
    return config_ptr->second;
}

void NodeConfig::setChainConfig(uint64_t chainHash, ChainConfig &&chainConfig)
{
    BOOST_LOG_TRIVIAL(info) << "Set config for chain #" << chainHash;
    mChainConfigs.insert(make_pair(chainHash, move(chainConfig)));
}

void NodeConfig::setChainConfig(uint64_t chainHash, const ChainConfig &chainConfig)
{
    setChainConfig(chainHash, ChainConfig(chainConfig));
}
