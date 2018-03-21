#ifndef BROADCAST_LOCAL_SOCKET_CONTROLLER_H
#define BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#endif //BROADCAST_LOCAL_SOCKET_CONTROLLER_H

#include "outer_socket.h"
#include "basic_socket.h"

#include "../src/node.h"
#include "../src/chain_config.h"

// has mChain map {id -> nodes}
// has remoteSocketController


// get message from local socket, parse mChain,
// run socketController.send() N times on mChain nodes
//

class InnerSocket : public BasicSocket {
    std::unordered_map<int, std::vector<Node>> *mChains;

    OuterSocket *outerSocket;

    const int LOCAL_PORT = 1122;

public:
    InnerSocket(OuterSocket &outerSocket, std::unordered_map<int, std::vector<Node>> &mChains, int port);


    void onReceive(size_t length) override;

//    void addMChain(ChainConfig &config);
};