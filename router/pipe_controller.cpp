#include "pipe_controller.h"

#include <regex>

using namespace std;
using namespace boost::filesystem;

PipeController::PipeController() {
    initPipes();
};

bool PipeController::hasPipe(const string pipeName) {
    return pipes.count(pipeName) > 0;
}

vector<string> PipeController::listFiles(string path) {
    boost::filesystem::path p(path);
    directory_iterator end_itr;
    vector<string> fileNames;

    for (directory_iterator itr(p); itr != end_itr; ++itr) {
        string file = itr->path().string();
        fileNames.push_back(file);
    }
    return fileNames;
}

void PipeController::initPipes() {
        vector<string> pipeNames = listFiles(FIFO_DIR);
    for (const string &name: pipeNames) {

        // removing path from pipe name
        string key = name.substr(FIFO_DIR.length() + 1);
//        cout << key << endl;
        pipes[key] = open(name.c_str(), O_RDWR);
    }
}

void PipeController::sendToPipe(const std::string pipeName, std::shared_ptr<Message> message) {
    if (!hasPipe(pipeName)) {
        cout << pipeName + " not found" << endl;
        return;
    }
    write(pipes[pipeName], &message, sizeof(message));
}