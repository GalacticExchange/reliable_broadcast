#include "socket.h"

using boost::asio::ip::udp;

#include <iostream>

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::vector;


Socket::Socket():
    mSocket(mIoService, udp::v4())
{

}

Socket::~Socket()
{
    mIoService.stop();
    if (mProcessThread.joinable())
    {
        mProcessThread.join();
    }
}

void Socket::send(const udp::endpoint &target,
                  shared_ptr<const vector<char>> buffer)
{
    mSocket.async_send_to(boost::asio::buffer(*buffer), target, [buffer](
            const boost::system::error_code &error,
            std::size_t) {
        if (error) {
            cerr << "Error on send: " << error.message() << endl;
        }
    });
}

void Socket::_startListen(size_t port)
{
    mSocket.bind(udp::endpoint(udp::v4(), port));
    asyncWaitForData();
    mProcessThread = std::thread([this]()
    {
        this->mIoService.run();
    });
}

void Socket::asyncWaitForData()
{
    mSocket.async_receive(boost::asio::buffer(mBuffer),
                          [this](boost::system::error_code error, std::size_t bytes_recvd)
    {
        if (!error)
        {
            this->onReceive(make_shared<vector<char>>(mBuffer.begin(), mBuffer.begin() + bytes_recvd));
        } else {
            cerr << "Error on send: " << error.message() << endl;
        }
        this->asyncWaitForData();
    });
}
