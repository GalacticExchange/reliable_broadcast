#ifndef NODE_H
#define NODE_H

#include <string>

class Node
{
    std::string mAddress;
    int mPort;

public:
    Node(const std::string &getAddress = "0.0.0.0", int getPort = 0);
    std::string getAddress() const;
    int getPort() const;
};

#endif // NODE_H
