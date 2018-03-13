#include <z3.h>
#include "pipe_controller.h"

using namespace std;
using namespace boost::filesystem;

PipeController::PipeController() {
    pipes = listFiles(FIFO_DIR);
};

bool PipeController::hasPipe(string pipeName) {
    // todo retrun bool val
    return std::find(pipes.begin(), pipes.end(), pipeName) != pipes.end();
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
