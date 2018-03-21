#include "inner_socket.h"

#ifndef BROADCAST_LOCAL_SOCKET_CONTROLLER_H
#define BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#endif //BROADCAST_LOCAL_SOCKET_CONTROLLER_H

using namespace std;

InnerSocket::InnerSocket(OuterSocket &outerSocket, unordered_map<int, std::vector<Node>> &mChains, int port)
        : BasicSocket(
        port) {

    this->outerSocket = &outerSocket;
    this->mChains = &mChains;

}


void InnerSocket::onReceive(size_t length) {
    cout << "Received message length: " << length << endl;

    shared_ptr<const vector<char>> mMessage = make_shared<vector<char >>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

//    vector<char> bytes = *mMessage;

    uint64_t chainName = Message::parseMChain(*mMessage);
    for (const Node &node : (*mChains)[chainName]) {
        boost::asio::ip::udp::endpoint endpoint = node.getEndpoint();
        outerSocket->send(endpoint, mMessage);
    }
}

