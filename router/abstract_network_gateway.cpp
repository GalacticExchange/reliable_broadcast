#include "abstract_network_gateway.h"



void AbstractNetworkGateway::startListen(size_t port, std::function<void (std::shared_ptr<std::vector<char> >)> callback)
{
    mOnReceiveCallback = callback;
    startListen(port);
}

void AbstractNetworkGateway::onReceive(std::shared_ptr<std::vector<char> > buffer)
{
    mOnReceiveCallback(buffer);
}
