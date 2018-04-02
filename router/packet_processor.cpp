#include "packet_processor.h"

using namespace std;

PacketProcessor::PacketProcessor(OuterSocket &outerSocket,
                                 std::unordered_map<int, ThreadSafeQueue<vector<char> > >  &queues,
                                 std::vector<Node> &nodes) :
    queues(queues)
{
    this->outerSocket = &outerSocket;

    for (auto &n : nodes) {
        this->nodes[n.getId()] = n;
    }
}


/**
 * blocking call
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void PacketProcessor::start() {
    for (;;) {
        cout << "tying to pop message..." << endl;
        sleep(5);
        //todo flag lock wait


//        // todo shuffle queues
        for (auto &idQueue : queues) {
            vector<vector<char>> drained = idQueue.second.drainTo(5); //todo create drainTo
            pendingMessages[idQueue.first].insert(pendingMessages[idQueue.first].end(), drained.begin(), drained.end());
        }

        send();
    }
}

#pragma clang diagnostic pop

void PacketProcessor::send() {
    cout << "pendingMessages size: " << pendingMessages.size() << endl;

//    for (auto &idVector: pendingMessages) {
//        shared_ptr<const vector<char>> packet = Packet::createPacket(idVector.second);
//        boost::asio::ip::udp::endpoint endpoint = nodes[idVector.first].getEndpoint();
//        outerSocket->send(endpoint, packet);
//    }
}
