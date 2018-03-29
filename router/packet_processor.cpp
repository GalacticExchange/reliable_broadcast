#include "packet_processor.h"

using namespace std;

PacketProcessor::PacketProcessor(OuterSocket &outerSocket, ThreadSafeQueue<std::vector<char>> &queue,
                                 std::vector<Node> &nodes) {
    this->outerSocket = &outerSocket;
    this->queue = &queue;
    this->chainNodes = nodes;
}


/**
 * blocking call
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void PacketProcessor::start() {
    for (;;) {
        vector<char> msg = queue->pop();
        pendingMessages.push_back(msg);
        checkAndSend();
    }
}

#pragma clang diagnostic pop

void PacketProcessor::checkAndSend() {
    if (pendingMessages.size() < 5 && !queue->isEmpty()) {
        return;
    }
    shared_ptr<const vector<char>> packet = Packet::createPacket(pendingMessages);

    for (const Node &node : chainNodes) {
        boost::asio::ip::udp::endpoint endpoint = node.getEndpoint();
        outerSocket->send(endpoint, packet);
    }
}
