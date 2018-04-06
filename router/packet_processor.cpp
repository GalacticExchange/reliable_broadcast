#include "packet_processor.h"

using namespace std;

PacketProcessor::PacketProcessor(OuterSocket &outerSocket,
                                 unordered_map<int, ThreadSafeQueue<vector<char> > > &queues,
                                 const vector<Node> &nodes) :
        queues(queues),
        semaphore(0) {
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

        std::stringstream q_sizes;
        for (auto key : getShuffledKeys()) {
            q_sizes << " " <<  queues[key].getSize();
        }

        cout << "== tying to pop message... queues sizes: " << q_sizes.str() << endl;

        semaphore.wait();
        cout << "################ woke up from wait! ##############" << endl;


//        std::unique_lock<std::mutex> lock(pMutex);
////        while (mQueue.empty()) {
//        condition.wait(lock);
////        }


        for (auto key : getShuffledKeys()) {
            vector<vector<char>> drained = queues[key].drainTo(5);
            if (!drained.empty()) {

                for (int i = 0; i < drained.size() - 1; i++) {
                    cout << "!!!!!!!! loop wait decrement !!!!!" << endl;
                    if (!semaphore.try_wait() ){
                        cout << "FUUUUUUU " << endl;
                        throw "FUUUUUU";
                    }

                    cout << "!!! loop wait decrement finished !!!" << endl;

                }
                pendingMessages[key].insert(pendingMessages[key].end(), drained.begin(),
                                            drained.end());
            }
        }

//        lock.unlock();

        send();

        cout << "iteration finished" << endl;
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

void PacketProcessor::notify() {
//    std::unique_lock<std::mutex> lock(pMutex);
////    todo?
////    lock.unlock();
//    condition.notify_one();
    semaphore.post();
}
