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
public:
    SocketController(int port);
    void send(boost::asio::ip::udp::endpoint &target,
              std::shared_ptr<const std::vector<char>> buffer);
    void listen();
private:
    void onReceive(size_t length);
    void asyncWaitForData();
    void syncWaitForData();
    void receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd);
};

#endif // SOCKETCONTROLLER_H
