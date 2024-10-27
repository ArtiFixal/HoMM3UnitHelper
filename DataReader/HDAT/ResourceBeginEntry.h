#pragma once
#include <fstream>
#include "GameObject.h"

namespace hdat
{
    /**
     * @brief Describes where block of resources begins, how many 
     * resources it have and last resource ID.
     */
    class ResourceBeginEntry
    {
    private:

        /**
         * @brief Position of the block first resource in the file.
         */
        std::streampos beginPos;

        /**
         * @brief Which one in order of all resources is this block 
         * first resource.
         */
        unsigned int resourceNumber;

        /**
         * @brief How many resources follow the block first resource.
         */
        unsigned int resourceCount;

        /**
         * @brief Highest possible ID for this block of resources.
         */
        ID resourceMaxID;

    public:
        ResourceBeginEntry();
        ResourceBeginEntry(std::streampos beginPos,unsigned int resourceNumber,
            unsigned int resourceCount, ID resourceMinID);
        ~ResourceBeginEntry(){};

        std::streampos getBeginPos();
        unsigned int getResourceNumber();
        unsigned int getResourceCount();
        ID getResourceMaxID();
    };   
}