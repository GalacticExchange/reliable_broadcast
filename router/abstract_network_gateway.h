#ifndef ABSTRACT_NETWORK_GATEWAY_H
#define ABSTRACT_NETWORK_GATEWAY_H

#include <functional>
#include <memory>
#include <vector>

#include <boost/asio.hpp>


class AbstractNetworkGateway
{
    std::function<void(std::shared_ptr<std::vector<char>>)> mOnReceiveCallback;

public:
    virtual ~AbstractNetworkGateway();
    virtual void send(const boost::asio::ip::udp::endpoint &target,
                      std::shared_ptr<const std::vector<char>> buffer) = 0;
    void startListen(size_t port, std::function<void(std::shared_ptr<std::vector<char>>)> callback);

protected:
    virtual void _startListen(size_t port) = 0;
    void onReceive(std::shared_ptr<std::vector<char>> buffer);
};

#endif // ABSTRACT_NETWORK_GATEWAY_H
