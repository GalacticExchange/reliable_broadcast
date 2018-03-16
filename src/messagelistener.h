#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include <fstream>
#include <memory>
#include <string>

#include "message.h"


class ReliableBroadcast;

class MessageListener
{    
//    int mPipeFileDescriptor;
//    boost::asio::io_service mIoService;
//    boost::asio::ip::udp::socket mSocket;
//    const size_t MAX_LENGTH = 512;
//    std::vector<char> mBuffer;
    std::fstream mInputStream;
    ReliableBroadcast &mOwner;
public:
    MessageListener(const std::string &pipeFileName, ReliableBroadcast &owner);

//    void send(boost::asio::ip::udp::endpoint &target,
//              std::shared_ptr<const std::vector<char>> buffer);
    void listen();
private:
    void onReceive(std::shared_ptr<vector<char>> buffer);
    void asyncWaitForData();
};

#endif // MESSAGELISTENER_H
