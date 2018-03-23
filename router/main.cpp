#include <thread>
#include "router.h"

using namespace std;

void sendTestMessage(Router &router);

void sendTestLocalMessage(InnerSocket &innerSocket);


void signalHandler(int signum) {
    cout << "Signal (" << signum << ") received.\n";
    //todo poll new mChains
}


int main() {
//    signal(SIGPOLL, signalHandler);

    Router router("/tmp/node/node_conf.json");

    thread routerThr([&router]() {
        router.start();
    });

    sendTestMessage(router);
    sendTestLocalMessage(router.getInnerSocket());
    routerThr.join();

    return 0;
}

void sendTestMessage(Router &router) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            static_cast<unsigned short>(router.getNodeConfig().getPort()));

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    router.getOuterSocket().send(targetEndpoint, charMessagePtr);
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