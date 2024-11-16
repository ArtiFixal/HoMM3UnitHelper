#pragma once
#include "DataPatcher.h"

namespace rhdat
{
    class DataPatch
    {
    public:
        virtual void patch(DataPatcher& patcher)=0;
    };
}
