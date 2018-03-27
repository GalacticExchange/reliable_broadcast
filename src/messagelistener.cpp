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
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

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

void MessageListener::listen()
{
    while (true)
    {
        uint16_t length;
        mInputStream.read(reinterpret_cast<char*>(&length), sizeof(length));
        cerr << "Ready to read " << length << "bytes" << endl;
        shared_ptr<vector<char>> buffer = make_shared<vector<char>>(length);
        mInputStream.read(&*buffer->begin(), length);

        onReceive(buffer);
    }
}

void MessageListener::onReceive(shared_ptr<vector<char>> buffer)
{
    BOOST_LOG_TRIVIAL(debug) << "Received " << buffer->size() << " bytes";
//    cerr << "Received data: [";
////    for (size_t i = 0; i < length; ++i)
//    for (size_t i = 0; i < 5; ++i)
//    {
//        if (i) cerr << ", ";
//        cerr << (int) mBuffer[i];
//    }
////    cerr << "]" << endl;
//    cerr << "..." << endl;

    mOwner.postMessage(buffer);
}
