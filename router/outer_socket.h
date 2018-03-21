#ifndef SOCKETCONTROLLER_H
#define SOCKETCONTROLLER_H

#include <string>

#include <boost/asio.hpp>

#include "pipe_controller.h"
#include "basic_socket.h"


class OuterSocket : public BasicSocket {

// todo pass params our pass pipeController from outside
    PipeController pipeController;
public:
    explicit OuterSocket(int port, std::string &fifoDir);

private:

    void onReceive(size_t length) override;
};

#endif // SOCKETCONTROLLER_H
