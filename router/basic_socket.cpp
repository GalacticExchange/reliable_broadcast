#include "basic_socket.h"

#include <iostream>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>


using std::make_shared;
using std::vector;


using std::cerr;
using std::endl;
using std::shared_ptr;

using boost::asio::ip::udp;


BasicSocket::BasicSocket(int port) :
        mBuffer(MAX_LENGTH),
        mSocket(mIoService, udp::endpoint(udp::v4(), port)),
        port(port)
         {
    asyncWaitForData();
}

void BasicSocket::send(udp::endpoint &target,
                            std::shared_ptr<const std::vector<char>> buffer) {
    mSocket.async_send_to(boost::asio::buffer(*buffer), target, [buffer](
            const boost::system::error_code &error,
            std::size_t) {
        if (error) {
            cerr << "Error on send: " << error << endl;
        }
    });
}

void BasicSocket::listen() {
    mIoService.run();
}

void BasicSocket::receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd) {
    if (!ec && bytes_recvd > 0) {
        this->onReceive(bytes_recvd);
    } else {
        cerr << "Error on receiving: " << ec << endl;
    }
    asyncWaitForData();
}

void BasicSocket::asyncWaitForData() {

    udp::endpoint sender;

    mSocket.async_receive_from(boost::asio::buffer(mBuffer), sender,
                               boost::bind(&BasicSocket::receiveHandler, this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));

}


void BasicSocket::uint64_to_string(uint64_t value, std::string &result) {
    result.clear();
    result.reserve(20); // max. 20 digits possible
    uint64_t q = value;
    do {
        result += "0123456789"[q % 10];
        q /= 10;
    } while (q);
    std::reverse(result.begin(), result.end());
}

int BasicSocket::getPort() {
    return port;
}

