#ifndef NODE_H
#define NODE_H

#include <string>
#include <boost/asio/ip/udp.hpp>

class Node {
    std::string mAddress;
    int mPort;
    boost::asio::ip::udp::endpoint targetEndpoint;
    int id;

public:

    Node() = default;
    Node(int nodeId, int port, const std::string &address = "0.0.0.0");

    std::string getAddress() const;

    int getPort() const;

    int getId() const;

    boost::asio::ip::udp::endpoint getEndpoint() const;
};

#endif // NODE_H
