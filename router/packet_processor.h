#ifndef BROADCAST_PACKET_PROCESSOR_H
#define BROADCAST_PACKET_PROCESSOR_H

#include "../src/threadsafequeue.h"
#include "outer_socket.h"
#include "../src/node.h"
#include "../src/message.h"


class PacketProcessor {

    OuterSocket *outerSocket;
    ThreadSafeQueue<std::vector<char>> *queue;
    std::vector<Node> chainNodes;
    std::vector<std::vector<char> > pendingMessages;

public:
    PacketProcessor(OuterSocket &outerSocket, ThreadSafeQueue<std::vector<char>> &queue, std::vector<Node> &chainNodes);

    void start();

    void process();

    std::shared_ptr<const std::vector<char>> assemblePacket();
};


#endif //BROADCAST_PACKET_PROCESSOR_H
