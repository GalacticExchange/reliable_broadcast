#ifndef SOCKETCONTROLLER_H
#define SOCKETCONTROLLER_H

#include <string>

#include <boost/asio.hpp>

#include "pipe_controller.h"


class SocketController
{
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mSocket;
    const size_t MAX_LENGTH = 512;
    std::vector<char> mBuffer;

    PipeController pipeController;
public:
    SocketController(int port);
    void send(boost::asio::ip::udp::endpoint &target,
              std::shared_ptr<const std::vector<char>> buffer);
    void listen();
private:
    void onReceive(size_t length);
    void asyncWaitForData();
//    void syncWaitForData();
    void receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd);
    void uint64_to_string(uint64_t value, std::string& result);
};

#endif // SOCKETCONTROLLER_H
