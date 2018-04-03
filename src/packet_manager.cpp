#include "packet_manager.h"
#include "reliablebroadcast.h"

using boost::asio::io_service;

using std::shared_ptr;
using std::vector;


PacketManager::PacketManager(io_service &io_service,
                             ReliableBroadcast &reliableBroadcast,
                             SocketController &socketController):
    mIoService(io_service),
    mReliableBroadcast(reliableBroadcast),
    mSocketController(socketController)
{

}

void PacketManager::asyncProcess(vector<char>::const_iterator begin,
                                 vector<char>::const_iterator end)
{
    mIoService.post([this, begin, end]()
    {
        this->mReliableBroadcast.asyncProcessMessage(
                    Message::parse(begin, end));
    });
}

void PacketManager::asyncBroadcast(shared_ptr<Message> message)
{
    throw std::logic_error("Not implemented");
}
