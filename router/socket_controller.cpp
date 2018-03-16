#include <iostream>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "socket_controller.h"

using std::make_shared;
using std::vector;


using std::cerr;
using std::endl;
using std::shared_ptr;

using boost::asio::ip::udp;


SocketController::SocketController(int port) :
        mSocket(mIoService, udp::endpoint(udp::v4(), port)),
        mBuffer(MAX_LENGTH) {
    asyncWaitForData();
}

void SocketController::send(udp::endpoint &target,
                            std::shared_ptr<const std::vector<char>> buffer) {
    mSocket.async_send_to(boost::asio::buffer(*buffer), target, [buffer](
            const boost::system::error_code &error,
            std::size_t) {
        if (error) {
            cerr << "Error on send: " << error << endl;
        }
    });
}

void SocketController::listen() {
    mIoService.run();
}

void SocketController::onReceive(size_t length) {
    std::cout << "Received message length: " << length << endl;

    // todo process message to mChain
    std::shared_ptr<const std::vector<char>> mMessage = make_shared<vector<char>>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

    vector<char> bytes = *mMessage;

//    Message message(1, 150, 3, 4, Message::MessageType::SEND, vector<char>());
//
//    vector<char> bytes = message.encode();

    std::string mChain;
    uint64_to_string(Message::parseMChain(bytes), mChain);

    pipeController.sendToPipe(mChain, bytes);

}


void SocketController::receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd) {
    std::cout << "Entered callback " << endl;
    if (!ec && bytes_recvd > 0) {
        this->onReceive(bytes_recvd);
    } else {
        cerr << "Error on receiving: " << ec << endl;
    }
    asyncWaitForData();
}

void SocketController::asyncWaitForData() {
    std::cout << "Entered asyncWaitForData()" << endl;

    udp::endpoint sender;

    mSocket.async_receive_from(boost::asio::buffer(mBuffer), sender,
                               boost::bind(&SocketController::receiveHandler, this, boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

//void SocketController::syncWaitForData() {
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


void SocketController::uint64_to_string(uint64_t value, std::string &result) {
    result.clear();
    result.reserve(20); // max. 20 digits possible
    uint64_t q = value;
    do {
        result += "0123456789"[q % 10];
        q /= 10;
    } while (q);
    std::reverse(result.begin(), result.end());
}

