#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include "message.h"

#include <fstream>
#include <memory>
#include <string>


class ReliableBroadcast;

class MessageListener
{    
    std::fstream mInputStream;
    ReliableBroadcast &mOwner;

public:
    MessageListener(const std::string &pipeFileName, ReliableBroadcast &owner);
    void listen();

private:
    void onReceive(std::shared_ptr<std::vector<char>> buffer);
    void asyncWaitForData();
};

#endif // MESSAGELISTENER_H
