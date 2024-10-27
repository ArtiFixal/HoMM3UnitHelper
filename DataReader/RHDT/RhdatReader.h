#pragma once
#include "Rhdat.h"
#include "BinaryReader.h"
#include "GameObject.h"
#include "Unit.h"
#include "IllegalFileExtensionException.h"
#include "ObjectNotFoundException.h"

namespace rhdat{

    /**
     * @brief Reads data from RHDAT file.
     */
    class RhdatReader: public BinaryReader
    {
    private:

        /**
         * @brief File header.
         */
        RhdatHeader header;
        EntryPtr unitDataPtr;

        Unit _readUnit(ID unit);

        /**
         * @brief Search for @code Unit with given ID.
         * 
         * @param unitID ID to Look for.
         * 
         * @return Found unit ID.
         * @throws ObjectNotFoundException If unit was not found.
         */
        ID findUnitByID(ID unitID);

        /**
         * @brief Search for @code Unit with given name.
         * 
         * @param unitName Name to look for.
         * 
         * @return Found unit ID.
         * @throws ObjectNotFoundException If unit was not found.
         */
        ID findUnitByName(string& unitName);

    public:
        RhdatReader() = default;
        RhdatReader(std::filesystem::path& pathToFile);
        ~RhdatReader(){};

        /**
         * @brief Reads Unit data with given ID.
         * 
         * @param unitID Which unit to read.
         * 
         * @return Read unit data.
         * @throws ObjectNotFoundException If unit was not found.
         */
        Unit readUnit(ID unitID);

        /**
         * @brief Reads Unit data with given name.
         * 
         * @param unitName Which unit to read.
         * 
         * @return Read unit data.
         * @throws ObjectNotFoundException If unit was not found.
         */
        Unit readUnit(string& unitName);
    };
}