#include "node.h"

std::string Node::getAddress() const
{
    return mAddress;
}

int Node::getPort() const
{
    return mPort;
}

Node::Node(const std::string &address, int port):
    mAddress(address),
    mPort(port)
{

}
