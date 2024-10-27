#pragma once
#include <stdexcept>
#include <filesystem>

class FileNotFoundException: public std::runtime_error
{
private:
    std::filesystem::path filePath;

public:
    FileNotFoundException(std::filesystem::path filePath): std::runtime_error("File not found: "+filePath.string()),filePath(filePath){}
    ~FileNotFoundException(){};

    std::filesystem::path getFilePath(){
        return filePath;
    }
};