#pragma once
#include <stdexcept>
#include "LodEntry.h"

namespace h3lod{
    class UnsupportedFileTypeException: public std::runtime_error
    {
    private:
        FileType fileType;
    public:
        UnsupportedFileTypeException(FileType fileType): std::runtime_error("Unsupported file type: "+std::to_string(static_cast<int>(fileType))),fileType(fileType){};
        ~UnsupportedFileTypeException(){};

        FileType getFileType(){
            return fileType;
        }
    };
}