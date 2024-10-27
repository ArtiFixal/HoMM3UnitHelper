#pragma once
#include "h3lod_defs.h"
#include "ObjectNotFoundException.h"

namespace h3lod
{
    class EntryNotFoundException: public ObjectNotFoundException
    {
    private:
        string entryName;
    public:
        EntryNotFoundException(string entryName): ObjectNotFoundException("LOD entry",entryName),entryName(entryName){};
        ~EntryNotFoundException(){};

        string getEntryName(){
            return entryName;
        }
    };
}
