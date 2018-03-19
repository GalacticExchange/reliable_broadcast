#include <thread>

#include "outer_socket.h"


using namespace std;

void sendTestMessage(OuterSocket &outerSocket);

int main() {

    OuterSocket outerSocket(1234);

    thread listener([&outerSocket]() {
        outerSocket.listen();
    });


    sleep(2);

    sendTestMessage(outerSocket);

    listener.join();
    return 0;
}

void sendTestMessage(OuterSocket &outerSocket) {

    std::string str = "hello world!";
    std::vector<char> data(str.begin(), str.end());

    Message message(1, 150, 12345, 4, Message::MessageType::SEND, static_cast<vector<char> &&>(data));

    boost::asio::ip::udp::endpoint targetEndpoint(
            boost::asio::ip::address::from_string("127.0.0.1"),
            1234);

    shared_ptr<vector<char>> charMessagePtr = make_shared<vector<char>>(message.encode());

    outerSocket.send(targetEndpoint, charMessagePtr);
}