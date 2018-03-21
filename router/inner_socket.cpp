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

    std::vector<char> msg = *mMessage;

    auto message = Message::parse(msg.begin(), msg.end());

    string str(msg.begin(), msg.end());

    uint64_t chainName = Message::parseMChain(*mMessage);
    for (const Node &node : (*mChains)[chainName]) {

        cout << "broadcasting msg: " << str << " ,to party: " << node.getPort() << endl;
        boost::asio::ip::udp::endpoint endpoint = node.getEndpoint();
        outerSocket->send(endpoint, mMessage);
    }
}

