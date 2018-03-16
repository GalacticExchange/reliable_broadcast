#include <fcntl.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

#include <boost/asio.hpp>

using boost::asio::ip::udp;

#include "message.h"
#include "messagelistener.h"
#include "reliablebroadcast.h"


MessageListener::MessageListener(const std::string &pipeFileName, ReliableBroadcast &owner):    
    mOwner(owner)
{
    bool fileExists;
    {
        ifstream f(pipeFileName.c_str());
        fileExists = f.good();
    }
    if (fileExists)
    {
        mInputStream.open(pipeFileName, ios::in | ios::out | ios::binary);
    } else {
        stringstream ss;
        ss << "Pipe " << pipeFileName << " was not found" << endl;
        string errorMessage;
        getline(ss, errorMessage);
        throw std::logic_error(errorMessage);
    }
}

//void MessageListener::send(udp::endpoint &target,
//                            std::shared_ptr<const std::vector<char>> buffer)
//{
//    mSocket.async_send_to(boost::asio::buffer(*buffer), target, [buffer](
//                          const boost::system::error_code& error,
//                          std::size_t)
//    {
//        if (error)
//        {
//            cerr << "Error on send: " << error << endl;
//        }
//    });
//}

void MessageListener::listen()
{
    while (true)
    {
        uint16_t length;
        mInputStream.read(reinterpret_cast<char*>(&length), sizeof(length));
        vector<char> buffer(length);
        mInputStream.read(&*buffer.begin(), length);

        shared_ptr<Message> message = Message::parse(buffer.begin(), buffer.end());
        onReceive(message);
    }
}

void MessageListener::onReceive(shared_ptr<Message> message)
{
    cerr << "Received message with nonce " << message->getNonce() << endl;

//    shared_ptr<Message> message = Message::parse(mBuffer.begin(), mBuffer.begin() + length);
////    cerr << "Received data: [";
//////    for (size_t i = 0; i < length; ++i)
////    for (size_t i = 0; i < 5; ++i)
////    {
////        if (i) cerr << ", ";
////        cerr << (int) mBuffer[i];
////    }
//////    cerr << "]" << endl;
////    cerr << "..." << endl;
//    mOwner.postMessage(message);
}

//void MessageListener::asyncWaitForData()
//{
//    udp::endpoint sender;
//    mSocket.async_receive_from(
//                boost::asio::buffer(mBuffer),
//                sender,
//                [this](boost::system::error_code ec, std::size_t bytes_recvd)
//    {
//        if (!ec && bytes_recvd > 0)
//        {
//            this->onReceive(bytes_recvd);
//        } else {
//            cerr << "Error on receiving: " << ec << endl;
//        }
//        asyncWaitForData();
//    });
//}
