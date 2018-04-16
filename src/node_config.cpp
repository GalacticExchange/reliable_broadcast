#include "file_utils.h"
#include "node_config.h"

#include <boost/log/trivial.hpp>

using boost::shared_lock;
using boost::shared_mutex;
using boost::unique_lock;

using std::cerr;
using std::endl;
using std::make_pair;
using std::make_shared;
using std::move;
using std::string;
using std::vector;


std::string NodeConfig::getRedisHost() const {
    return mRedisHost;
}

size_t NodeConfig::getRedisPort() const {
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

    readChainConfigs();
}

void NodeConfig::readChainConfigs() {
    vector<string> configs = FileUtils::listFiles(mChainConfigDir);
    for (const string &configPath : configs) {
        ChainConfig chainConfig(configPath);
        setChainConfig(chainConfig.getMChainHash(), move(chainConfig));
    }
    cerr << endl;
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

std::shared_ptr<const ChainConfig> NodeConfig::getChainConfig(uint64_t chainHash) const {
    shared_lock<shared_mutex> lock(mChainConfigsMutex);
    auto config_iterator = mChainConfigs.find(chainHash);
    if (config_iterator == mChainConfigs.end()) {
        throw std::logic_error("Request config of non existing chain");
    }
    return config_iterator->second;
}

void NodeConfig::setChainConfig(uint64_t chainHash, ChainConfig &&chainConfig) {
    BOOST_LOG_TRIVIAL(info) << "Set config for chain #" << chainHash;
    unique_lock<shared_mutex> lock(mChainConfigsMutex);
    mChainConfigs[chainHash] = make_shared<ChainConfig>(move(chainConfig));
}

void NodeConfig::setChainConfig(uint64_t chainHash, const ChainConfig &chainConfig) {
    setChainConfig(chainHash, ChainConfig(chainConfig));
}

std::vector<ChainConfig> NodeConfig::getConfigs() const {
    vector<ChainConfig> v;
    for (auto keyValue : mChainConfigs) {
        v.push_back(*(keyValue.second));
    }

    return v;
}
