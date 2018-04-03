#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include "reliablebroadcast.h"


class SocketController
{
public:
    void asyncSend(const boost::asio::ip::udp::endpoint &target,
                   std::shared_ptr<const std::vector<char>> buffer);
};

class PacketManager
{
public:
    PacketManager(boost::asio::io_service &io_service,
                  ReliableBroadcast &reliableBroadcast,
                  SocketController &SocketController);

    void asyncProcess(std::shared_ptr<const std::vector<char>> buffer);
    void asyncBroadcast(std::shared_ptr<Message> message);
};

#endif // PACKET_MANAGER_H
