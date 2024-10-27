#include "ResourceDescriptor.h"

namespace hdat
{
    ResourceDescriptor::ResourceDescriptor(): resourceType(ResourceType::Unknown),resourceID(-1){}

    ResourceDescriptor::ResourceDescriptor(ResourceType type, ID resourceID):resourceType(type),resourceID(resourceID){}

    ResourceDescriptor::ResourceDescriptor(string type, ID resourceID): resourceID(resourceID)
    {
        if(type=="monst")
            resourceType=ResourceType::Monster;
        else
            resourceType=ResourceType::Unknown;
    }

    bool ResourceDescriptor::operator==(ResourceDescriptor &resDesc)
    {
        return (this->resourceType==resDesc.getResourceType() && resourceID==resDesc.getResourceID());
    }

    bool ResourceDescriptor::operator!=(ResourceDescriptor &resDesc)
    {
        return !operator==(resDesc);
    }

    ID ResourceDescriptor::getResourceID()
    {
        return resourceID;
    }

    ResourceType ResourceDescriptor::getResourceType()
    {
        return resourceType;
    }
}
