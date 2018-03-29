#ifndef BROADCAST_LOCAL_SOCKET_CONTROLLER_H
#define BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#include "outer_socket.h"
#include "basic_socket.h"

#include "../src/node.h"
#include "../src/chain_config.h"
#include "../src/threadsafequeue.h"
#include <thread>
#include "packet_processor.h"

// has mChain map {id -> nodes}
// has remoteSocketController


// get message from local socket, parse mChain,
// run socketController.send() N times on mChain nodes
//

class InnerSocket : public BasicSocket {
    std::unordered_map<int, std::vector<Node>> *mChains;

    OuterSocket *outerSocket;

    int port;

    std::unordered_map<uint64_t, ThreadSafeQueue<std::vector<char> > > sendQueues;
    std::unordered_map<uint64_t, std::thread> senders;


public:
    InnerSocket(OuterSocket &outerSocket, std::unordered_map<int, std::vector<Node>> &mChains, int port);


    void onReceive(size_t length) override;


    void addToPending(const std::vector<char> &msg, uint64_t chainName);

    void processPending(uint64_t chainName);
};

#endif //BROADCAST_LOCAL_SOCKET_CONTROLLER_H
