#include "pipe_controller.h"
#include <regex>

using namespace std;
using namespace boost::filesystem;

PipeController::PipeController(string &fifoDir) {
    initPipes(fifoDir);
};

bool PipeController::hasPipe(const string pipeName) {
    return pipes.count(pipeName) > 0;
}

void PipeController::initPipes(string &fifoDir) {
    vector<string> pipeNames = FileUtils::listFiles(fifoDir);
    for (const string &name: pipeNames) {

        // removing path from pipe name
//        string key = name.substr(FIFO_DIR.length() + 1);
        string key = FileUtils::getBaseName(name);
//        cout << key << endl;
        pipes[key] = open(name.c_str(), O_RDWR);
    }
}

void PipeController::sendToPipe(const std::string &pipeName, vector<char> bytes) {
    if (!hasPipe(pipeName)) {
        cout << "Pipe: " << pipeName + " - not found" << endl;
        return;
    }
    cout << "Pipe: sending length " << endl;
    auto length = static_cast<uint16_t>(bytes.size());
    write(pipes[pipeName], reinterpret_cast<char *>(&length), sizeof(length));
    cout << "Pipe: sending message " << endl;
    write(pipes[pipeName], bytes.data(), bytes.size());
}