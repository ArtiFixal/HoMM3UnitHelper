#include "LodReader.h"

namespace h3lod{

    LodReader::LodReader(std::filesystem::path& filePath)
    {
        if(!std::filesystem::exists(filePath))
            throw FileNotFoundException(filePath);
        lodFile=std::ifstream(filePath,std::ios_base::binary);
        LodHeader header;
        // Check file extension
        lodFile.read(reinterpret_cast<char*>(&header),sizeof(header));
        if(header.magicBytes!=LOD_FILE_MAGIC_BYTES)
            throw IllegalFileExtensionException(LOD_FILE_MAGIC_BYTES,header.magicBytes);
        fileCount=header.fileCount;
        // Skip LOD metadata
        fileBegin=lodFile.tellg()+(std::streampos)80;
        lodFile.seekg(fileBegin);
    }

    LodReader::~LodReader()
    {
        if(lodFile.is_open())
            lodFile.close();
    }

    LodFile LodReader::getFile(string entryName)
    {
        LodEntry found=findEntry(entryName);
        string data=readCompressedFile(found);
        return LodFile(found,data);
    }

    std::unique_ptr<TmpFile> LodReader::extractFileTemporary(std::string entryName, std::filesystem::path& extractPath)
    {
        std::unique_ptr<TmpFile> tmp=std::make_unique<TmpFile>(extractPath,std::ios_base::out);
        LodFile toExtract=getFile(entryName);
        tmp->write(toExtract.getData().c_str(),toExtract.getData().size());
        return tmp;
    }

    inline LodEntry LodReader::findEntry(string entryName)
    {
        return findEntry(entryName,fileBegin);
    }

    LodEntry LodReader::findEntry(string entryName, std::streampos startFrom)
    {
        LodEntry entry;
        lodFile.seekg(startFrom);
        int maxRetries=fileCount;
        while(maxRetries--&&entryName!=entry.fileName)
            lodFile.read(reinterpret_cast<char*>(&entry),sizeof(LodEntry));
        if(entry.fileName!=entryName)
            throw EntryNotFoundException(entryName);
        return entry;
    }

    string LodReader::readCompressedFile(LodEntry fileHeader)
    {
        lodFile.seekg(fileHeader.filePointer);
        char *buff=new char[fileHeader.compressedSize];
        lodFile.read(buff,fileHeader.compressedSize);
        return string(buff,fileHeader.compressedSize);
    }
}