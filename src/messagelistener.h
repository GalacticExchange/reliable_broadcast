#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H



#include <string>

#include "../capnproto/skale_message.capnp.h"


class ReliableBroadcast;

class MessageListener
{    
    int mPipeFileDescriptor;
//    boost::asio::io_service mIoService;
//    boost::asio::ip::udp::socket mSocket;
//    const size_t MAX_LENGTH = 512;
//    std::vector<char> mBuffer;
    ReliableBroadcast &mOwner;
public:
    MessageListener(const std::string &pipeFileName, ReliableBroadcast &owner);
    virtual ~MessageListener();
//    void send(boost::asio::ip::udp::endpoint &target,
//              std::shared_ptr<const std::vector<char>> buffer);
    void listen();
private:
    void onReceive(const SkaleMessage::Reader &skaleMessageReader);
    void asyncWaitForData();
};

#endif // MESSAGELISTENER_H
