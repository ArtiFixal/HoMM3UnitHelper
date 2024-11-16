#pragma once
#include <vector>
#include <memory>
#include "Rhdat.h"
#include "GameObject.h"

namespace rhdat{
    
    /**
     * @brief Stores single type of @code GameObject data to be saved 
     * in file.
     */
    struct WriteEntry
    {
        /**
         * @brief Type of data to be stored by this entry.
         */
        EntryType entryType;
        std::vector<std::unique_ptr<GameObject>> objectsToSave;

        WriteEntry(EntryType type):entryType(type){}
        WriteEntry(const WriteEntry&)=delete;
        WriteEntry(WriteEntry&&)=default;
    };
}