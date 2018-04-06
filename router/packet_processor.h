#ifndef BROADCAST_PACKET_PROCESSOR_H
#define BROADCAST_PACKET_PROCESSOR_H

#include "../src/threadsafequeue.h"
#include "outer_socket.h"
#include "../src/node.h"
#include "packet.h"
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

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

    std::condition_variable condition;
    std::mutex pMutex;

    boost::interprocess::interprocess_semaphore semaphore;

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
