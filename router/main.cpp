#include <thread>

#include "socket_controller.h"


using namespace std;

void sendTestMessage(SocketController &controller);

int main() {

    SocketController controller(1234);

    thread listener([&controller]() {
        controller.listen();
    });


    sleep(2);

    sendTestMessage(controller);

    listener.join();
    return 0;
}

void sendTestMessage(SocketController &controller) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 12345, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            1234);

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    controller.send(targetEndpoint, charMessagePtr);
}