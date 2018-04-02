#include "inner_socket.h"

using namespace std;


InnerSocket::InnerSocket(OuterSocket &outerSocket, unordered_map<uint64_t, vector<Node> > &mChains, int port)
        : BasicSocket(port) {

    this->outerSocket = &outerSocket;
    this->mChains = &mChains;


}

void InnerSocket::onReceive(size_t length) {
    cout << "Received message length: " << length << endl;

    shared_ptr<const vector<char>> mMessage = make_shared<vector<char >>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

    vector<char> msg = *mMessage;

    auto message = Message::parse(msg.begin(), msg.end());

//    string str(msg.begin(), msg.end());

    uint64_t chainName = Message::parseMChain(*mMessage);
    cerr << "Received broadcast request to " << chainName << " mChain" << endl;
//    sendQueues[chainName]->push(msg); //todo

//    for (const Node &node : (*mChains)[chainName]) {
//
////        cout << "broadcasting msg: " << str << " ,to party: " << node.getPort() << endl;
//        boost::asio::ip::udp::endpoint endpoint = node.getEndpoint();
//        outerSocket->send(endpoint, mMessage);
//    }
}

void InnerSocket::updateQueues() {

    for (auto &keyValue : *(mChains)) {
        for (Node &n : keyValue.second) {
            // initializing queues
            sendQueues[n.getId()];
        }
    }

//    for (auto &keyValue : sendQueues) {
//        cout << "sendQueue key: " << keyValue.first << endl;
//    }

    packetProcessorThr = thread([this] {
        cout << "test" << endl;
        PacketProcessor p(*(this->outerSocket),
                          this->sendQueues,
                          (*(this->mChains))[mChains->begin()->first]);
        p.start();
    });

}
