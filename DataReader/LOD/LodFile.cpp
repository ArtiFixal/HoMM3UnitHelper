#include "LodFile.h"

namespace h3lod{

    LodFile::LodFile(LodEntry fileEntry, string& compressedData)
    {
        this->fileEntry=fileEntry;
        data.resize(fileEntry.uncompressedSize);
        z_stream strm={Z_NULL};
        strm.avail_in=fileEntry.compressedSize;
        strm.avail_out=fileEntry.uncompressedSize;
        strm.next_in=reinterpret_cast<Bytef*>(&compressedData[0]);
        strm.next_out=reinterpret_cast<Bytef*>(&data[0]);
        inflateInit(&strm);
        inflate(&strm,Z_FINISH);
        inflateEnd(&strm);
    }
    
    string& LodFile::getData()
    {
        return data;
    }
          
    LodEntry LodFile::getFileEntry()
    {
        return fileEntry;
    }
}