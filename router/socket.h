#ifndef SOCKET_H
#define SOCKET_H

#include "abstract_network_gateway.h"

#include <thread>

class Socket : public AbstractNetworkGateway
{
    const size_t MAX_LENGTH = 512;
    boost::asio::io_service mIoService;
    std::vector<char> mBuffer;
    boost::asio::ip::udp::socket mSocket;
    std::thread mProcessThread;

public:
    Socket();
    ~Socket();
    void send(const boost::asio::ip::udp::endpoint &target,
              std::shared_ptr<const std::vector<char>> buffer) override;

protected:
    void _startListen(size_t port) override;

private:
    void asyncWaitForData();
};

#endif // SOCKET_H
