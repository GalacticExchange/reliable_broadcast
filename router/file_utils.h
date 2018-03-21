#ifndef BROADCAST_FILE_UTILS_H
#define BROADCAST_FILE_UTILS_H

#include <iostream>
#include <vector>
#include "boost/filesystem.hpp"



class FileUtils{


public:
    static std::vector<std::string> listFiles(std::string path);
    static std::string getBaseName(std::string filePath);
};


#endif //BROADCAST_FILE_UTILS_H
