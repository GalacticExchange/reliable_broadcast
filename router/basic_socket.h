

#ifndef BROADCAST_BASICSOCKETCONTROLLER_H
#define BROADCAST_BASICSOCKETCONTROLLER_H

#include <string>

#include <boost/asio.hpp>
#include "../src/message.h"


class BasicSocket {

protected:
    boost::asio::io_service mIoService;
    boost::asio::ip::udp::socket mSocket;
    const size_t MAX_LENGTH = 512;
    std::vector<char> mBuffer;
    int port;

public:
    explicit BasicSocket(int port);


    void send(boost::asio::ip::udp::endpoint &target,
              std::shared_ptr<const std::vector<char>> buffer);

    void listen();

    int getPort();

protected:
    virtual void onReceive(size_t length)= 0;

    void asyncWaitForData();

//    void syncWaitForData();
    void receiveHandler(const boost::system::error_code &ec, std::size_t bytes_recvd);

    void uint64_to_string(uint64_t value, std::string &result);

};


#endif //BROADCAST_BASICSOCKETCONTROLLER_H
