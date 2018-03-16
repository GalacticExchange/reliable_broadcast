#include "../capnproto/internal_message.capnp.h"
#include "message.h"
#include "messagelistener.h"
#include "reliablebroadcast.h"

#include <boost/asio.hpp>
#include <capnp/serialize.h>

using boost::asio::ip::udp;

#include <fcntl.h>

#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::ifstream;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;


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
        stringstream ss;
        ss << "Pipe " << pipeFileName << " was not found" << endl;
        string errorMessage;
        getline(ss, errorMessage);

        throw std::logic_error(errorMessage);
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
    vector<capnp::StreamFdMessageReader> messages;
    vector<InternalMessage::Reader> readers;
    capnp::StreamFdMessageReader message(mPipeFileDescriptor);
    while (true)
    {
        try
        {

            InternalMessage::Reader reader = message.getRoot<InternalMessage>();

//            messages.resize(messages.size() + 1);

//            readers.emplace_back(reader);

//            for (auto &r : readers)
//            {
//                cerr << ' ' << r.getNonce();
//            }
//            cerr << endl;

            cerr << reader.getNonce() << endl;

//            messages.emplace_back(std::move(capnp::StreamFdMessageReader(mPipeFileDescriptor)));

//            bool first = true;
//            for (auto &message : messages)
//            {
//                if (first) first = false;
//                else cerr << ", ";
//                InternalMessage::Reader internalMessageReader = message.getRoot<InternalMessage>();
//                cerr << internalMessageReader.getNonce();
//            }
//            cerr << endl;

//            InternalMessage::Reader internalMessageReader = message.getRoot<InternalMessage>();
//            InternalMessage::Reader internalMessageReader2 = message.getRoot<InternalMessage>();
//            cerr << internalMessageReader.getNonce() << ' ' << internalMessageReader2.getNonce() << endl;
//            onReceive(internalMessageReader);
        } catch(...) {
            cerr << "Can't parse message" << endl;
        }
    }
}

void MessageListener::onReceive(InternalMessage &&internalMessage)
{
//    cerr << "Received message with nonce " << skaleMessageReader.getNonce() << endl;
//    shared_ptr<Message> message = make_shared<Message>(move(internalMessage));
//////    cerr << "Received data: [";
////////    for (size_t i = 0; i < length; ++i)
//////    for (size_t i = 0; i < 5; ++i)
//////    {
//////        if (i) cerr << ", ";
//////        cerr << (int) mBuffer[i];
//////    }
////////    cerr << "]" << endl;
//////    cerr << "..." << endl;
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
