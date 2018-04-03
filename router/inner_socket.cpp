#include "inner_socket.h"

using namespace std;


InnerSocket::InnerSocket(OuterSocket &outerSocket, unordered_map<uint64_t, vector<Node> > &mChains, int port)
        : BasicSocket(port) {

    this->outerSocket = &outerSocket;
    this->mChains = &mChains;
    this->packetProcessor = make_shared<PacketProcessor>(*(this->outerSocket),
                                                         this->sendQueues);


}

void InnerSocket::onReceive(size_t length) {
    cout << "Received message length: " << length << endl;

    shared_ptr<const vector<char>> mMessage = make_shared<vector<char >>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

    vector<char> msg = *mMessage;

    uint64_t chainName = Message::parseMChain(*mMessage);
//    cout << "Received broadcast request to " << chainName << " mChain" << endl;

    for (const Node &node : (*mChains)[chainName]) {
        sendQueues[node.getId()].push(msg);
    }

    // todo
//    packetProcessor->notify();
}

void InnerSocket::updateQueues() {

    for (auto &keyValue : *(mChains)) {
        for (Node &n : keyValue.second) {
            // initializing queues
            sendQueues[n.getId()];

            packetProcessor->addNode(n);
        }
    }

    packetProcessorThr = thread([this] {
        this->packetProcessor->start();
    });

}
