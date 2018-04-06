#include <thread>
#include "router.h"

using namespace std;

void sendTestMessage(Router &router);

void sendTestLocalMessage(InnerSocket &innerSocket);


void signalHandler(int signum) {
    cout << "Signal (" << signum << ") received.\n";
    //todo poll new mChains
}


int main(int argc, char *argv[]) {
//    signal(SIGPOLL, signalHandler);


    string nodeConfigPath;
    if (argc >= 2) {
        nodeConfigPath = string(argv[1]);
    } else {
//        nodeConfigPath = "/tmp/node/node.json";
        nodeConfigPath = "../node_config.json";
    }

    Router router(nodeConfigPath);

    thread routerThr([&router]() {
        router.start();
    });

    sendTestMessage(router);
//    sendTestLocalMessage(router.getInnerSocket());
    routerThr.join();

    return 0;
}

void sendTestMessage(Router &router) {

    std::string str = "hello world!";
    std::string str2 = "hello 2world!";
    std::vector<char> data(str.begin(), str.end());
    std::vector<char> data2(str2.begin(), str2.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));
    Message message2(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data2));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            static_cast<unsigned short>(router.getNodeConfig().getPort()));

//    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());
//    router.getOuterSocket().send(targetEndpoint, charMessagePtr);

    vector< vector<char> > msgs;
    msgs.push_back(message.encode());
    msgs.push_back(message2.encode());
    shared_ptr<const vector<char>> packet = Packet::createPacket(msgs);

    vector<char> ztest = *packet;
    string s(ztest.begin(), ztest.end());
    cout << "Test sending packet: " << s << endl;

    router.getOuterSocket().send(targetEndpoint, packet);

}


void sendTestLocalMessage(InnerSocket &innerSocket) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            static_cast<unsigned short>(innerSocket.getPort()));

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    innerSocket.send(targetEndpoint, charMessagePtr);
}