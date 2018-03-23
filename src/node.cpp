#include "node.h"

std::string Node::getAddress() const {
    return mAddress;
}

int Node::getPort() const {
    return mPort;
}

Node::Node(int nodeId, int port, const std::string &address) :
        mAddress(address),
        mPort(port),
        targetEndpoint(boost::asio::ip::address::from_string(address),
                       static_cast<unsigned short>(port)),
        id(nodeId) {

}

boost::asio::ip::udp::endpoint Node::getEndpoint() const {
    return targetEndpoint;
}

int Node::getId() const {
    return id;
}
