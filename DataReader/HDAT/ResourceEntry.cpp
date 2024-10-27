#include "ResourceEntry.h"

namespace hdat
{
    ResourceEntry::ResourceEntry(): beginPos(-1),resourceNumber(-1),resourceCount(0),resourceMaxID(0)
    {}

    ResourceEntry::ResourceEntry(std::streampos beginPos,unsigned int resourceNumber,unsigned resourceCount,ID resourceMinID): beginPos(beginPos),resourceNumber(resourceNumber),resourceCount(resourceCount),resourceMaxID(resourceMinID+resourceCount){}

    std::streampos ResourceEntry::getBeginPos()
    {
        return beginPos;
    }

    unsigned int ResourceEntry::getResourceNumber()
    {
        return resourceNumber;
    }

    unsigned int ResourceEntry::getResourceCount(){
        return resourceCount;
    }

    ID ResourceEntry::getResourceMaxID(){
        return resourceMaxID;
    }
}
