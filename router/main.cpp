#include <thread>

//#include "outer_socket.h"
#include "router.h"

using namespace std;

void sendTestMessage(OuterSocket &outerSocket);


void signalHandler(int signum) {
    cout << "Signal (" << signum << ") received.\n";
    //todo poll new mChains
}


int main() {
//    signal(SIGPOLL, signalHandler);

    Router router("../node_config.json");

    thread routerThr([&router]() {
        router.start();
    });

    sendTestMessage(router.getOuterSocket());

    routerThr.join();

    return 0;
}

void sendTestMessage(OuterSocket &outerSocket) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 12345, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            Router::UDP_OUTER_PORT);

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    outerSocket.send(targetEndpoint, charMessagePtr);
}