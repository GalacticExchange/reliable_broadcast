#ifndef SOCKET_H
#define SOCKET_H

#include "abstract_network_gateway.h"

#include <thread>

class Socket : public AbstractNetworkGateway
{
    const size_t MAX_LENGTH = 512;
    boost::asio::io_service mIoService;
    std::vector<char> mBuffer;
    std::thread mProcessThread;

public:    
    void send(const boost::asio::ip::udp::endpoint &target,
              std::shared_ptr<const std::vector<char>> buffer) override;

protected:
    void startListen(size_t port) override;

private:
    void asyncWaitForData();
};

#endif // SOCKET_H
