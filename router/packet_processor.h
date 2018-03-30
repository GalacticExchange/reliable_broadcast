#ifndef BROADCAST_PACKET_PROCESSOR_H
#define BROADCAST_PACKET_PROCESSOR_H

#include "../src/threadsafequeue.h"
#include "outer_socket.h"
#include "../src/node.h"
#include "packet.h"


using namespace std;

class PacketProcessor {

    OuterSocket *outerSocket;

    //{nodeId, queue}
    unordered_map<int, ThreadSafeQueue<vector<char>>> queues;

    //{nodeId, node}
    unordered_map<int, Node> nodes;

    //{nodeId, msgs}
    unordered_map<int, vector<vector<char> > > pendingMessages;

public:
    PacketProcessor(OuterSocket &outerSocket, unordered_map<int, ThreadSafeQueue<vector<char>>> &queues,
                    vector<Node> &nodes);

    void start();

    void send();

};


#endif //BROADCAST_PACKET_PROCESSOR_H
