#pragma once
#include "Hdat.h"
#include "ResourceType.h"
#include "GameObject.h"

namespace hdat
{
    /**
     * @brief Describes single resource in HDAT file.
     */
    class ResourceDescriptor
    {
    private:
        ID resourceID;
        
    public:
        ResourceType resourceType;

        ResourceDescriptor();
        ResourceDescriptor(ResourceType type,ID resourceID);

        /**
         * @brief Translates string to @code ResourceType and creates @code ResourceDescriptor.
         * 
         * @param type Resource type without ID.
         */
        ResourceDescriptor(string type,ID resourceID);
        ~ResourceDescriptor(){};

        bool operator==(ResourceDescriptor& resDesc);
        bool operator!=(ResourceDescriptor& resDesc);

        ID getResourceID();
        ResourceType getResourceType();
    };
}
