#include "packet_manager.h"
#include "packet_message.h"
#include "reliablebroadcast.h"

#include <boost/log/trivial.hpp>

using boost::asio::io_service;

#include <iostream>

using std::cerr;
using std::dynamic_pointer_cast;
using std::endl;
using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::vector;


PacketManager::PacketManager(io_service &io_service,
                             ReliableBroadcast &reliableBroadcast,
                             SocketController &socketController,
                             const NodeConfig &nodeConfig) :
        mIoService(io_service),
        mReliableBroadcast(reliableBroadcast),
        mSocketController(socketController),
        mNodeConfig(nodeConfig) {

}

void PacketManager::asyncProcess(vector<char>::const_iterator begin,
                                 vector<char>::const_iterator end) {
    // TODO: do not block input buffer on parsing
    shared_ptr<AbstractMessage> message = AbstractMessage::parse(begin, end);
    if (message) {
        asyncProcess(message);
    } else {
        BOOST_LOG_TRIVIAL(debug) << "Received bad message";
    }
}

void PacketManager::asyncProcess(std::shared_ptr<AbstractMessage> message) {
    mIoService.post([this, message]() {
        if (message->getType() <= AbstractMessage::MessageType::READY) {
            this->mReliableBroadcast.asyncProcessMessage(dynamic_pointer_cast<Message>(message));

        } else if (message->getType() == AbstractMessage::MessageType::PACKET) {
            for (auto packetedMessage : dynamic_pointer_cast<PacketMessage>(message)->getMessages()) {
                asyncProcess(packetedMessage);
            }
        } else {
            throw std::logic_error("Can't process message with type " +
                                   std::to_string(message->getType()));
        }
    });
}

ostream &operator<<(ostream &os, const vector<char> &buffer) {
    bool first = true;
    for (char character : buffer) {
        if (first) {
            first = false;
        } else {
            os << ' ';
        }
        if ((character >= 'a' && character <= 'z') ||
            (character >= 'A' && character <= 'Z')) {
            os << character;
        } else {
            os << static_cast<int>(character);
        }
    }
    return os;
}

void PacketManager::asyncBroadcast(shared_ptr<Message> message) {
    mIoService.post([this, message]() {
        shared_ptr<vector<char>> buffer;
        if (rand() & 1) {
            buffer = make_shared<vector<char>>(move(message->encode()));
        } else {
            PacketMessage packet;
            packet.addMessage(message);
            buffer = make_shared<vector<char>>(move(packet.encode()));
        }

        auto chainConfig_ptr = mNodeConfig.getChainConfig(message->getMChainHash());
        for (const auto &id_node : chainConfig_ptr->getNodes()) {
            if (mNodeConfig.getId() != id_node.first) {
                mSocketController.asyncSend(id_node.second.getEndpoint(), buffer);
            }
        }
    });
}
