#include "inner_socket.h"

#ifndef BROADCAST_LOCAL_SOCKET_CONTROLLER_H
#define BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#endif //BROADCAST_LOCAL_SOCKET_CONTROLLER_H

using namespace std;

InnerSocket::InnerSocket(OuterSocket &outerSocket, unordered_map<int, std::vector<Node>> &mChains) : BasicSocket(
        LOCAL_PORT) {

    this->outerSocket = &outerSocket;
    this->mChains = &mChains;

}


void InnerSocket::addMChain(ChainConfig &config) {
    std::vector<Node> nodes;
    nodes.reserve(config.getNodes().size());

    for (auto kv : config.getNodes()) {
        nodes.push_back(kv.second);
    }

    //todo

    (*mChains)[config.getId()] = nodes;
}


void InnerSocket::onReceive(size_t length) {
    cout << "Received message length: " << length << endl;

    shared_ptr<const vector<char>> mMessage = make_shared<vector<char >>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

    vector<char> bytes = *mMessage;

    std::string mChain;
    uint64_to_string(Message::parseMChain(bytes), mChain);
}

