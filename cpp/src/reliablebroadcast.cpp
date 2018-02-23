#include <iostream>
#include <memory>
#include <thread>
#include <vector>

using std::dynamic_pointer_cast;
using std::make_shared;
using std::shared_ptr;
using std::thread;
using std::vector;

#include "externalmessage.h"
#include "reliablebroadcast.h"
#include "session.h"

ReliableBroadcast::ReliableBroadcast(int id, const std::unordered_map<int, Node> &nodes):
    mId(id),
    mNodes(nodes),
    mSocketController(mNodes[id].getPort(), *this)
{
}

void ReliableBroadcast::start()
{
    asyncProcessMessage();
    vector<thread> pool;
    for (size_t i = 0; i < thread::hardware_concurrency(); ++i)
    {
        pool.emplace_back([this]
        {
            this->mIoService.run();
        });
    }
    mSocketController.listen();
    for (auto &thread : pool)
    {
        thread.join();
    }
}

void ReliableBroadcast::postMessage(std::shared_ptr<Message> message)
{
    mMessageQueue.push(message);
}

void ReliableBroadcast::asyncProcessMessage()
{
    mIoService.post([this]()
    {
        shared_ptr<Message> message = mMessageQueue.pop();
        asyncProcessMessage();
        processMessage(message);
    });
}

void ReliableBroadcast::processMessage(shared_ptr<Message> message)
{
    if (message->getType() == Message::MessageType::MESSAGE)
    {
        shared_ptr<ExternalMessage> externalMessage =
                dynamic_pointer_cast<ExternalMessage>(message);

        shared_ptr<Session> session = make_shared<Session>(*this, externalMessage);
    }
}

void ReliableBroadcast::broadcast(std::shared_ptr<InternalMessage> message)
{
    throw std::logic_error("unimplemented");
}
