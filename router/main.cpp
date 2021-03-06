#include <thread>
#include "router.h"

using namespace std;

void sendTestMessage(Router &router);

void sendTestLocalMessage(size_t port);


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
//        nodeConfigPath = "/tmp/node/node_conf.json";
        nodeConfigPath = "/tmp/node/node.json";
    }

    Router router(nodeConfigPath);

    thread routerThr([&router]() {
        router.start();
    });

    sendTestMessage(router);
    sendTestLocalMessage(router.getNodeConfig().getLocalPort());
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


void sendTestLocalMessage(size_t port) {
    Socket socket;
//    socket.startListen(5556, [](std::shared_ptr<std::vector<char>> buffer){});

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 1234, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            static_cast<unsigned short>(port));

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    socket.send(targetEndpoint, charMessagePtr);
    sleep(1);
}
