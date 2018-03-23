#include "socket.h"

using std::make_shared;
using std::shared_ptr;
using std::vector;


void Socket::send(const boost::asio::ip::udp::endpoint &target,
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

void Socket::startListen(size_t port)
{
    mProcessThread = std::thread([this]()
    {
        boost::asio::ip::udp socket(this->mIoService, port);
        this->asyncWaitForData();
        mIoService.run();
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
