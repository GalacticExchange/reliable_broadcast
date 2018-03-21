#include "node.h"

std::string Node::getAddress() const {
    return mAddress;
}

int Node::getPort() const {
    return mPort;
}

Node::Node(const std::string &address, int port) :
        mAddress(address),
        mPort(port),
        targetEndpoint(boost::asio::ip::address::from_string(address),
                       port) {

}

boost::asio::ip::udp::endpoint Node::getEndpoint() const {
    return targetEndpoint;
}
