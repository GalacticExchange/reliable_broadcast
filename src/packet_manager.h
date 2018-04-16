#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include "message.h"
#include "socket_controller.h"
#include "node_config.h"
#include "threadsafequeue.h"

using namespace std;

class ReliableBroadcast;

class PacketManager
{
    boost::asio::io_service &mIoService;
    ReliableBroadcast &mReliableBroadcast;
    SocketController &mSocketController;
    const NodeConfig &mNodeConfig;

    //{nodeId, queue}
    unordered_map<int, ThreadSafeQueue<vector<char> > > queues;

    //{nodeId, node}
    unordered_map<int, Node> nodes;

    //{nodeId, msgs[]}
    unordered_map<int, vector<vector<char> > > pendingMessages;

public:
    PacketManager(boost::asio::io_service &io_service,
                  ReliableBroadcast &reliableBroadcast,
                  SocketController &socketController,
                  const NodeConfig &nodeConfig);

    void asyncProcess(vector<char>::const_iterator begin,
                      vector<char>::const_iterator end);
    void asyncProcess(shared_ptr<AbstractMessage> message);
    void asyncBroadcast(shared_ptr<Message> message);

    void setNodes(vector<Node> nodes);

    void addNode(const Node &n);

    void updateQueues();
};

#endif // PACKET_MANAGER_H
