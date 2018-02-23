#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::shared_ptr;

#include <boost/asio.hpp>

using boost::asio::ip::udp;

#include "message.h"
#include "socketcontroller.h"


SocketController::SocketController(int port, ReliableBroadcast &owner):
    mSocket(mIoService, udp::endpoint(udp::v4(), port)),
    mBuffer(MAX_LENGTH),
    mOwner(owner)
{
    asyncWaitForData();
}

void SocketController::listen()
{
    mIoService.run();
}

void SocketController::onReceive(size_t length)
{
    shared_ptr<Message> message = Message::parse(mBuffer.begin(), mBuffer.begin() + length);
    cerr << "Received data: [";
    for (size_t i = 0; i < length; ++i)
    {
        if (i) cerr << ", ";
        cerr << (int) mBuffer[i];
    }
    cerr << "]" << endl;
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
