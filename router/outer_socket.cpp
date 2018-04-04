#include <iostream>
#include <memory>
#include <vector>

#include <boost/asio.hpp>

#include "outer_socket.h"

using std::make_shared;
using std::vector;


using std::cerr;
using std::endl;
using std::shared_ptr;

using boost::asio::ip::udp;


OuterSocket::OuterSocket(int port, std::string &fifoDir) :
        BasicSocket(port),
        pipeController(fifoDir) {
}


void OuterSocket::onReceive(size_t length) {
    std::cout << "Received message length: " << length << endl;

    std::shared_ptr<const std::vector<char>> mMessage = make_shared<vector<char>>(
            *mBuffer.begin() ? mBuffer.begin() : mBuffer.begin() + 1, mBuffer.begin() + length);

    vector<vector<char> > parsed = *(Packet::parsePacket(mMessage).get());
    for (vector<char> &rawMsg : parsed) {
        std::string mChain;
        uint64_to_string(Message::parseMChain(rawMsg), mChain);
        pipeController.sendToPipe(mChain, rawMsg);
    }


}



