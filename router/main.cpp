#include <thread>
#include "router.h"

using namespace std;

void sendTestMessage(OuterSocket &outerSocket);
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

    sendTestMessage(router.getOuterSocket());
    sendTestLocalMessage(router.getInnerSocket());
    routerThr.join();

    return 0;
}

void sendTestMessage(OuterSocket &outerSocket) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            Router::UDP_OUTER_PORT);

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    outerSocket.send(targetEndpoint, charMessagePtr);
}


void sendTestLocalMessage(InnerSocket &innerSocket) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            Router::UDP_INNER_PORT);

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    innerSocket.send(targetEndpoint, charMessagePtr);
}