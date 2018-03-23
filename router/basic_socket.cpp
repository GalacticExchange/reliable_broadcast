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
//    std::cout << "sending msg" << std::endl;
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

//void BasicSocket::onReceive(size_t length) {
//    std::cout << "Received message length: " << length << endl;
//
//    std::shared_ptr<const std::vector<char>> mMessage = make_shared<vector<char>>(
//            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);
//
//    vector<char> bytes = *mMessage;
//
//    std::string mChain;
//    uint64_to_string(Message::parseMChain(bytes), mChain);
//
//    pipeController.sendToPipe(mChain, bytes);
//}


void BasicSocket::receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd) {
//    std::cout << "Entered callback " << endl;
    if (!ec && bytes_recvd > 0) {
        this->onReceive(bytes_recvd);
    } else {
        cerr << "Error on receiving: " << ec << endl;
    }
    asyncWaitForData();
}

void BasicSocket::asyncWaitForData() {
//    std::cout << "Entered asyncWaitForData()" << endl;

    udp::endpoint sender;

    mSocket.async_receive_from(boost::asio::buffer(mBuffer), sender,
                               boost::bind(&BasicSocket::receiveHandler, this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));

//    mSocket.async_receive_from(
//            boost::asio::buffer(mBuffer),
//            sender,
//            [this](boost::system::error_code ec, std::size_t bytes_recvd)
//            {
//                if (!ec && bytes_recvd > 0)
//                {
//                    this->onReceive(bytes_recvd);
//                } else {
//                    cerr << "Error on receiving: " << ec << endl;
//                }
//                asyncWaitForData();
//            });
}

//void BasicSocket::syncWaitForData() {
//    std::cout << "Entered syncWaitForData()" << endl;
//    udp::endpoint sender;
//
//
//    for (;;){
//        std::size_t received_len = mSocket.receive(boost::asio::buffer(mBuffer));
//
//
//        std::shared_ptr<const std::vector<char>> mMessage =
//                make_shared<vector<char>>(*mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1,
//                                          mBuffer.begin() + received_len);
//
//        std::vector<char> msg = *mMessage;
//        std::cout << msg.data() << endl;
//    }
//
//}


//void BasicSocket::onReceive(size_t length) {
//    std::cout << "Received message length: " << length << endl;
//
//    // todo process message to mChain
//    std::shared_ptr<const std::vector<char>> mMessage = make_shared<vector<char>>(
//            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);
//
//    vector<char> bytes = *mMessage;
//
//    std::string mChain;
//    uint64_to_string(Message::parseMChain(bytes), mChain);
//
////    pipeController.sendToPipe(mChain, bytes);
//
//}


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

