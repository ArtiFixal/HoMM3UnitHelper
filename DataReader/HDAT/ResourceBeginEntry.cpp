#include "ResourceBeginEntry.h"

namespace hdat
{
    ResourceBeginEntry::ResourceBeginEntry(): ResourceBeginEntry(-1,-1,0,0){}

    ResourceBeginEntry::ResourceBeginEntry(std::streampos beginPos,unsigned int resourceNumber,unsigned int resourceCount,ID resourceMinID): beginPos(beginPos),resourceNumber(resourceNumber),resourceCount(resourceCount),resourceMaxID(resourceMinID+resourceCount){}

    std::streampos ResourceBeginEntry::getBeginPos()
    {
        return beginPos;
    }

    unsigned int ResourceBeginEntry::getResourceNumber()
    {
        return resourceNumber;
    }

    unsigned int ResourceBeginEntry::getResourceCount(){
        return resourceCount;
    }

    ID ResourceBeginEntry::getResourceMaxID(){
        return resourceMaxID;
    }
}
