#pragma once
#include <type_traits>
#include <memory>
#include "h3lod_defs.h"
#include "LodFile.h"
#include "GameObject.h"

namespace h3lod{

    /**
     * @brief Abstract base class for readers of .LOD archive files.
     */
    template<typename T>
    class GameDataReader
    {
    protected:
        LodFile& file;
        std::istringstream reader;

    public:
        GameDataReader(LodFile& fileToRead);
        ~GameDataReader();

        bool isEof() const;

        /**
         * @brief Reads next data.
         * @return Next data entry.
         */
        virtual std::unique_ptr<T> readData() = 0;
        virtual std::unique_ptr<T> readData(uint objectID) = 0;
        virtual std::unique_ptr<T> readData(string objectName) = 0;
    };

    template <typename T>
    inline GameDataReader<T>::GameDataReader(LodFile &fileToRead) : file(fileToRead)
    {
        static_assert(std::is_base_of<GameObject,T>::value,"GameDataReader template type have to be derived from GameObject");
        reader=std::istringstream(file.getData(),std::ios_base::in);
    }

    template <typename T>
    inline GameDataReader<T>::~GameDataReader() {}

    template <typename T>
    inline bool GameDataReader<T>::isEof() const
    {
        return reader.eof();
    }
}