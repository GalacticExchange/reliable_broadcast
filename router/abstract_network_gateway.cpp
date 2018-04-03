#include "abstract_network_gateway.h"

#include <iostream>

using std::cerr;
using std::endl;


AbstractNetworkGateway::~AbstractNetworkGateway()
{

}

void AbstractNetworkGateway::startListen(size_t port, std::function<void (std::shared_ptr<std::vector<char> >)> callback)
{
    mOnReceiveCallback = callback;
    _startListen(port);
}

void AbstractNetworkGateway::onReceive(std::shared_ptr<std::vector<char>> buffer)
{
    mOnReceiveCallback(buffer);
}
