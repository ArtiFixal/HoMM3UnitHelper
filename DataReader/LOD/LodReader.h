#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <memory>
#include "Lod.h"
#include "LodEntry.h"
#include "LodFile.h"
#include "TmpFile.h"
#include "UnitExtended.h"
#include "GameDataReader.h"
#include "UnitDataReader.h"
#include "FileNotFoundException.h"
#include "IllegalFileExtensionException.h"
#include "UnsupportedFileTypeException.h"
#include "EntryNotFoundException.h"

namespace h3lod{

    /**
     * @brief Reads data from .LOD archives.
     */
    class LodReader
    {
        private:
            std::ifstream lodFile;
            uint fileCount;
            std::streampos fileBegin;

            /**
             * @brief Search for entry with given name from beginig of file.
             * 
             * @param entryName Name to look for.
             * 
             * @return Found entry.
             */
            inline LodEntry findEntry(string entryName);

            /**
             * @brief Search for entry with given name from given position.
             * 
             * @param entryName Name to look for.
             * @param startFrom From where to look for.
             * 
             * @return Found entry.
             */
            LodEntry findEntry(string entryName,std::streampos startFrom);

            string readCompressedFile(LodEntry fileHeader);

        public:

            /**
             * @param filePath Path to the LOD file.
             */
            LodReader(std::filesystem::path& filePath);
            ~LodReader();

            /**
             * @brief Searches for entry with given name and uncompresses it.
             * @param entryName File to uncompress.
             * 
             * @return Uncompressed file.
             * @throws EntryNotFoundException If file entry was not found.
             */
            LodFile getFile(string entryName);

            /**
             * @brief Finds file and creates reader for that file.
             * 
             * @tparam T Data type read from that file.
             * @param entryName File to uncompress.
             * 
             * @return Reader able to read given data type.
             * @throws EntryNotFoundException If file entry was not found.
             */
            template<typename T>
            std::unique_ptr<GameDataReader<T>> getFileReader(string entryName);

            /**
             * @brief Extracts file from .LOD archive to given path and deletes it when no longer used.
             * 
             * @param entryName What to extract.
             * @param extractPath Where to extract.
             * 
             * @return Temporary extracted file stream.
             * @throws EntryNotFoundException If file entry was not found.
             */
            std::unique_ptr<TmpFile> extractFileTemporary(string entryName, std::filesystem::path& extractPath);
    };

    template <>
    inline std::unique_ptr<GameDataReader<UnitExtended>> LodReader::getFileReader(string entryName){
        LodFile found=getFile(entryName);
        return std::make_unique<UnitDataReader>(found);
    }

    template <typename T>
    inline std::unique_ptr<GameDataReader<T>> LodReader::getFileReader(string entryName)
    {
        LodFile found=getFile(entryName);
        switch (found.getFileEntry().fileType)
        {
        default:
            throw UnsupportedFileTypeException(found.getFileEntry().fileType);
        }
    }
}