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
    unordered_map<int, ThreadSafeQueue<vector<char> > > &queues;

    //{nodeId, node}
    unordered_map<int, Node> nodes;

    //{nodeId, msgs[]}
    unordered_map<int, vector<vector<char> > > pendingMessages;

    default_random_engine rng;

    std::condition_variable cv;
    std::mutex mx;

public:
    PacketProcessor(OuterSocket &outerSocket, unordered_map<int, ThreadSafeQueue<vector<char> > > &queues,
                    const vector<Node> &nodes = vector<Node>());

    void start();

    void send();

    vector<int> getShuffledKeys();

    void addNode(Node &n);

    void notify();
};


#endif //BROADCAST_PACKET_PROCESSOR_H
