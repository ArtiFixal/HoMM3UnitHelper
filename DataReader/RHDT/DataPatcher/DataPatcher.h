#pragma once
#include "WriteEntry.h"
#include "NoEntryException.h"
#include "ObjectNotFoundException.h"

namespace rhdat
{
    class DataPatcher
    {
    private:
        std::vector<WriteEntry>& entries;

        WriteEntry& getWriteEntry(EntryType type);

    public:
        DataPatcher(std::vector<WriteEntry>& entries);
        ~DataPatcher(){};

        std::unique_ptr<GameObject>& getObject(EntryType type,ID objectID);
    };   
}