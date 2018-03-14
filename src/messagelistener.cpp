#include <fcntl.h>

#include <fstream>
#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::ifstream;
using std::shared_ptr;

#include <boost/asio.hpp>
#include <capnp/serialize.h>

using boost::asio::ip::udp;

#include "../capnproto/skale_message.capnp.h"
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
        mPipeFileDescriptor = open(pipeFileName.c_str(), O_RDWR);
    } else {
        throw std::logic_error("Pipe was not found");
    }
}

MessageListener::~MessageListener()
{
    if (mPipeFileDescriptor)
    {
        close(mPipeFileDescriptor);
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
        capnp::StreamFdMessageReader message(mPipeFileDescriptor);
        SkaleMessage::Reader skaleMessageReader = message.getRoot<SkaleMessage>();
        onReceive(skaleMessageReader);
    }
}

void MessageListener::onReceive(const SkaleMessage::Reader &skaleMessageReader)
{
    cerr << "Received message with nonce " << skaleMessageReader.getNonce() << endl;
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
