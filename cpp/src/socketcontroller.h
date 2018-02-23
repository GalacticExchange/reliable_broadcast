#ifndef SOCKETCONTROLLER_H
#define SOCKETCONTROLLER_H

#include <string>

#include <boost/asio.hpp>

class ReliableBroadcast;

class SocketController
{
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mSocket;
    const size_t MAX_LENGTH = 512;
    std::vector<char> mBuffer;
    ReliableBroadcast &mOwner;
public:
    SocketController(int port, ReliableBroadcast &owner);
    void send(const std::vector<char> &buffer);
    void listen();
private:
    void onReceive(size_t length);
    void asyncWaitForData();
};

#endif // SOCKETCONTROLLER_H
