#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include "message.h"
#include "socket_controller.h"
#include "node_config.h"


class ReliableBroadcast;

class PacketManager
{
    boost::asio::io_service &mIoService;
    ReliableBroadcast &mReliableBroadcast;
    SocketController &mSocketController;
    const NodeConfig &mNodeConfig;

public:
    PacketManager(boost::asio::io_service &io_service,
                  ReliableBroadcast &reliableBroadcast,
                  SocketController &socketController,
                  const NodeConfig &nodeConfig);

    void asyncProcess(std::vector<char>::const_iterator begin,
                      std::vector<char>::const_iterator end);
    void asyncBroadcast(std::shared_ptr<Message> message);
};

#endif // PACKET_MANAGER_H
