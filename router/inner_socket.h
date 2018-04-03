#ifndef BROADCAST_LOCAL_SOCKET_CONTROLLER_H
#define BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#include <thread>

#include "outer_socket.h"

#include "basic_socket.h"
#include "../src/node.h"
#include "../src/chain_config.h"
#include "../src/threadsafequeue.h"
#include "packet_processor.h"


using namespace std;

class InnerSocket : public BasicSocket {
    unordered_map<uint64_t, vector<Node>> *mChains;

    OuterSocket *outerSocket;
    unordered_map<int, ThreadSafeQueue <vector<char> > > sendQueues;
    thread packetProcessorThr;
    shared_ptr<PacketProcessor> packetProcessor;


//    PacketProcessor packetProcessor;


public:
    InnerSocket(OuterSocket &outerSocket, unordered_map<uint64_t, vector<Node>> &mChains, int port);

    void updateQueues();

    void onReceive(size_t length) override;


};

#endif //BROADCAST_LOCAL_SOCKET_CONTROLLER_H
