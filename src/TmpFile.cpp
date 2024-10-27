#include "TmpFile.h"

TmpFile::TmpFile(std::filesystem::path& filePath,std::ios_base::openmode openMode): std::fstream(filePath,openMode), filePath(filePath){}

TmpFile::~TmpFile()
{
    if(is_open())
        close();
    if(std::filesystem::exists(filePath))
        std::filesystem::remove(filePath);
}

std::filesystem::path& TmpFile::getFilePath()
{
    return filePath;
}
