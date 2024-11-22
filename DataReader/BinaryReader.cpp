#include "BinaryReader.h"

BinaryReader::BinaryReader(std::filesystem::path &pathToFile) : pathToFile(pathToFile)
{
    reader=std::ifstream(pathToFile,std::ios_base::binary);
}

BinaryReader::~BinaryReader()
{
    if(reader.is_open())
        reader.close();
}

std::filesystem::path& BinaryReader::getPathToFile()
{
    return pathToFile;
}
