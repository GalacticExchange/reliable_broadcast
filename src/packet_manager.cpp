#include "packet_manager.h"
#include "reliablebroadcast.h"

using boost::asio::io_service;

using std::make_shared;
using std::shared_ptr;
using std::vector;


PacketManager::PacketManager(io_service &io_service,
                             ReliableBroadcast &reliableBroadcast,
                             SocketController &socketController,
                             const NodeConfig &nodeConfig):
    mIoService(io_service),
    mReliableBroadcast(reliableBroadcast),
    mSocketController(socketController),
    mNodeConfig(nodeConfig)
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
    mIoService.post([this, message]()
    {
        auto chainConfig_ptr = mNodeConfig.getChainConfig(message->getMChainHash());
        shared_ptr<vector<char>> buffer = make_shared<vector<char>>(move(message->encode()));
        for (const auto &id_node : chainConfig_ptr->getNodes())
        {
            if (mNodeConfig.getId() != id_node.first)
            {
                mSocketController.asyncSend(id_node.second.getEndpoint(), buffer);
            }
        }
    });
}
