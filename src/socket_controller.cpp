#include "message.h"
#include "reliablebroadcast.h"
#include "socket_controller.h"

#include <boost/asio.hpp>

using boost::asio::ip::udp;

#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::function;
using std::make_shared;
using std::shared_ptr;
using std::vector;


SocketController::SocketController(int port):
    mSocket(mIoService, udp::endpoint(udp::v4(), port)),
    mBuffer(MAX_LENGTH)
{

}

void SocketController::asyncSend(const udp::endpoint &target,
                                 shared_ptr<const vector<char>> buffer)
{
    mSocket.async_send_to(boost::asio::buffer(*buffer), target, [buffer](
                          const boost::system::error_code& error,
                          std::size_t)
    {
        if (error)
        {
            cerr << "Error on send: " << error << endl;
        }
    });
}

void SocketController::listen(function<void(std::vector<char>::const_iterator begin,
                                            std::vector<char>::const_iterator end)> handler)
{
    mOnReceiveHandler = handler;
    asyncWaitForData();
    mIoService.run();
}

void SocketController::onReceive(size_t length)
{
    mOnReceiveHandler(mBuffer.begin(), mBuffer.begin() + length);
}

void SocketController::asyncWaitForData()
{
    udp::endpoint sender;
    mSocket.async_receive_from(
                boost::asio::buffer(mBuffer),
                sender,
                [this](boost::system::error_code ec, std::size_t bytes_recvd)
    {
        if (!ec && bytes_recvd > 0)
        {
            this->onReceive(bytes_recvd);
        } else {
            cerr << "Error on receiving: " << ec << endl;
        }
        asyncWaitForData();
    });
}
