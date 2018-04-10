#include "file_utils.h"
#include <regex>


using namespace std;
using namespace boost::filesystem;

std::vector<string> FileUtils::listFiles(string path) {

    boost::filesystem::path p(path);
    directory_iterator end_itr;
    vector<string> fileNames;

    for (directory_iterator itr(p); itr != end_itr; ++itr) {
        string file = itr->path().string();
        fileNames.push_back(file);
    }
    return fileNames;
}

//todo test
std::string FileUtils::getBaseName(std::string filePath) {
    path p(filePath);
    return p.filename().string();
}

