#include "DataPatcher.h"

namespace rhdat
{
    WriteEntry &DataPatcher::getWriteEntry(EntryType type)
    {
        for(auto& e:entries)
        {
            if(e.entryType==type)
                return e;
        }
        throw NoEntryException(type);
    }

    DataPatcher::DataPatcher(std::vector<WriteEntry> &entries): entries(entries){}

    std::unique_ptr<GameObject> &DataPatcher::getObject(EntryType type, ID objectID)
    {
        WriteEntry& entryOfType=getWriteEntry(type);
        for(auto& o:entryOfType.objectsToSave)
        {
            if(o->getID()==objectID)
                return o;
        }
        throw ObjectNotFoundException("Patch object",objectID);
    }
}
