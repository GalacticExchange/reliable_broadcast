#include "packet_processor.h"

using namespace std;

PacketProcessor::PacketProcessor(OuterSocket &outerSocket,
                                 unordered_map<int, ThreadSafeQueue<vector<char> > > &queues,
                                 const vector<Node> &nodes) :
        queues(queues) {
    this->outerSocket = &outerSocket;

    for (auto &n : nodes) {
        this->nodes[n.getId()] = n;
    }
    rng = default_random_engine{static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count())};
}


/**
 * blocking call
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void PacketProcessor::start() {
    for (;;) {
        cout << "tying to pop message..." << endl;

        sleep(5); //todo flag lock wait


        for (auto key : getShuffledKeys()){
            cout << key << endl;
            vector<vector<char>> drained = queues[key].drainTo(5);
            if (!drained.empty()) {
                pendingMessages[key].insert(pendingMessages[key].end(), drained.begin(),
                                                      drained.end());
            }
        }

        send();
    }
}

#pragma clang diagnostic pop

void PacketProcessor::send() {

    if (pendingMessages.empty()) {
        cout << "pendingMessages is empty, returning.. " << endl; // todo
        return;
    }

    for (auto &nodeId_Vector: pendingMessages) {
        shared_ptr<const vector<char>> packet = Packet::createPacket(nodeId_Vector.second);
        boost::asio::ip::udp::endpoint endpoint = nodes[nodeId_Vector.first].getEndpoint();
        outerSocket->send(endpoint, packet);
    }

    pendingMessages.clear();
}

vector<int> PacketProcessor::getShuffledKeys() {
    vector<int> keys;

    for (auto const &element : queues) {
        keys.push_back(element.first);
    }

    std::shuffle(keys.begin(), keys.end(), rng);
    return keys;
}

void PacketProcessor::addNode(Node &n) {
    this->nodes[n.getId()] = n;
}