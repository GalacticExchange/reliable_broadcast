#include "packet_manager.h"

using boost::asio::io_service;

using std::shared_ptr;
using std::vector;


PacketManager::PacketManager(io_service &io_service,
                             ReliableBroadcast &reliableBroadcast,
                             SocketController &SocketController)
{
    throw std::logic_error("Not implemented");
}

void PacketManager::asyncProcess(shared_ptr<const vector<char> > buffer)
{
    throw std::logic_error("Not implemented");
}

void PacketManager::asyncBroadcast(shared_ptr<Message> message)
{
    throw std::logic_error("Not implemented");
}
