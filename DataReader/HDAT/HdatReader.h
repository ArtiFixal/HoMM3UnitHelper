#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include "Hdat.h"
#include "BinaryReader.h"
#include "Unit.h"
#include "ResourceDescriptor.h"
#include "ResourceBeginEntry.h"
#include "IllegalFileExtensionException.h"
#include "ObjectNotFoundException.h"

namespace hdat{

    /**
     * @brief Reads data from HotA HDAT file.
     */
    class HdatReader: public BinaryReader
    {
    private:
        /**
         * @brief Lowest possible Unit ID in the file.
         */
        const int MIN_UNIT_ID=150;
        
        /**
         * @brief File header.
         */
        HdatHeader header;
        unsigned int currObjectNumber;
        ResourceBeginEntry unitsBegin;
        
        void _createResourceEntries();

        /**
         * @brief Skips entire @code Unit data directly to the next @code ResourceDescriptor.
         */
        void _skipUnitData();

        /**
         * @brief Skips partially @code Unit data directly to the name.
         */
        void _skipUnitDataByName();
        Unit _assignUnitData(ID unitID,string name);

        /**
         * @brief Sets reader position to the begining of the resource block.
         */
        void goToResourceBegin(ResourceBeginEntry& resourceToGoTo);

        /**
         * @brief Searches for the resource in a HDAT file.
         * 
         * @param resDesc What to look for.
         * @param resEntry Where to look for.
         * 
         * @return Descriptor of found resource.
         */
        ResourceDescriptor findResource(ResourceDescriptor resDesc,ResourceBeginEntry& resEntry);
        ResourceDescriptor readResourceDescriptor();
        
    public:
        /**
         * @param pathToData Path to hota .hdat file.
         */
        HdatReader(std::filesystem::path& pathToData);
        ~HdatReader(){};

        /**
         * @brief Reads @code Unit searching by ID.
         * 
         * @param unitID ID to look for.
         * 
         * @return Read Unit.
         * @throws ObjectNotFoundException If object was not found or ID is 
         * less than 150.
         */
        Unit readUnit(ID unitID);

        /**
         * @brief Reads @code Unit searching by name.
         * 
         * @param unitName Name to look for.
         * 
         * @return Read Unit.
         * @throws ObjectNotFoundException If object was not found.
         */
        Unit readUnit(string& unitName);
    };
}