#ifndef SOCKET_CONTROLLER_H
#define SOCKET_CONTROLLER_H

#include <boost/asio.hpp>

#include <functional>
#include <string>


class SocketController
{
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mSocket;
    const size_t MAX_LENGTH = 2048;
    std::vector<char> mBuffer;
    std::function<void(std::vector<char>::const_iterator begin,
                       std::vector<char>::const_iterator end)> mOnReceiveHandler;

public:
    SocketController(int port);
    void asyncSend(const boost::asio::ip::udp::endpoint &target,
                   std::shared_ptr<const std::vector<char>> buffer);
    void listen(std::function<void(std::vector<char>::const_iterator begin,
                                   std::vector<char>::const_iterator end)> handler);
private:
    void onReceive(size_t length);
    void asyncWaitForData();
};
#endif // SOCKET_CONTROLLER_H
