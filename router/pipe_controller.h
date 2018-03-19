#ifndef BROADCAST_PIPE_CONTROLLER_H
#define BROADCAST_PIPE_CONTROLLER_H

#endif //BROADCAST_PIPE_CONTROLLER_H

#include <iostream>
#include <vector>
#include "boost/filesystem.hpp"
#include <iostream>


#include <unordered_map>
#include <fcntl.h>

#include "../src/message.h"


class PipeController {

    const std::string FIFO_DIR = "/tmp/skale_fifo";

//    std::vector<std::string> pipes;
    std::unordered_map<std::string, int> pipes;

public:
    PipeController();

    std::vector<std::string> listFiles(std::string path);

    bool hasPipe(std::string pipeName);

    void sendToPipe(const std::string &pipeName, std::vector<char> bytes);

private:
    void initPipes();
};