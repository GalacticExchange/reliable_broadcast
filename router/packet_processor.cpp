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
        process();
    }
}

#pragma clang diagnostic pop

void PacketProcessor::process() {
    if (pendingMessages.size() < 5 && !queue->isEmpty()) {
        return;
    }
    shared_ptr<const vector<char>> packet = assemblePacket();

    for (const Node &node : chainNodes) {
        boost::asio::ip::udp::endpoint endpoint = node.getEndpoint();
        outerSocket->send(endpoint, packet);
    }
}

shared_ptr<const vector<char>> PacketProcessor::assemblePacket() {
    vector<char> packet;

    auto amount = static_cast<int>(pendingMessages.size());
    vector<int> lengths;

    for (const auto &msg: pendingMessages) {
        packet.insert(packet.begin(), msg.begin(), msg.end());
        lengths.push_back(static_cast<int &&>(msg.size()));
    }
    size_t total_size = sizeof(amount) +
                        lengths.size() * sizeof(int) +
                        packet.size();

    vector<char> buffer(total_size);
    size_t offset = 0;
    Message::write<typeof(amount)>(buffer.begin(), offset, amount);
    offset += sizeof(amount);
    for (int len : lengths) {
        Message::write<typeof(len)>(buffer.begin(), offset, len);
        offset += sizeof(len);
    }
    copy(packet.begin(), packet.end(), buffer.begin() + offset);

//    buffer[offset] = static_cast<char>(mType);
//    offset += 1;
//    copy(mData.begin(), mData.end(), buffer.begin() + offset);


    return make_shared<vector<char >>(*buffer.begin() ? buffer.begin() : buffer.begin() + 1,
                                      buffer.begin() + buffer.size());
}


