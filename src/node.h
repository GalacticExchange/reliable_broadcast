#ifndef NODE_H
#define NODE_H

#include <string>
#include <boost/asio/ip/udp.hpp>

class Node
{
    std::string mAddress;
    int mPort;
    boost::asio::ip::udp::endpoint targetEndpoint;

public:
    Node(const std::string &getAddress = "0.0.0.0", int getPort = 0);
    std::string getAddress() const;
    int getPort() const;
    boost::asio::ip::udp::endpoint getEndpoint() const;
};

#endif // NODE_H
