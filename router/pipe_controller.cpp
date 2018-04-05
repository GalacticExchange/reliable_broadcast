#include "pipe_controller.h"
#include <regex>

using namespace std;
using namespace boost::filesystem;

PipeController::PipeController(const string &fifoDir) {
    this->fifoDir = fifoDir;
    updatePipes();
};

bool PipeController::hasPipe(const string pipeName) {
    return pipes.count(pipeName) > 0;
}

void PipeController::updatePipes() {
    vector<string> pipeNames = FileUtils::listFiles(fifoDir);
    for (const string &name: pipeNames) {
        string key = FileUtils::getBaseName(name);
        if (!hasPipe(key)) {
//            cout << "Adding fd of pipe " << key << endl;
            pipes[key] = open(name.c_str(), O_RDWR);
        }
    }
}

void PipeController::sendToPipe(const string &pipeName, vector<char> bytes) {
    if (!hasPipe(pipeName)) {
        cout << "Pipe: " << pipeName + " - not found" << endl;
        return;
    }
//    cout << "Pipe: sending length " << endl;
    auto length = static_cast<uint16_t>(bytes.size());
    write(pipes[pipeName], reinterpret_cast<char *>(&length), sizeof(length));
//    cout << "Pipe: sending message " << endl;
    write(pipes[pipeName], bytes.data(), bytes.size());
}