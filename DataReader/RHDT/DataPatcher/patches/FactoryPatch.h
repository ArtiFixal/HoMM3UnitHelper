#pragma once
#include "DataPatch.h"

namespace rhdat
{
    /**
     * @brief Patch containing changes made to the game data by adding Factory faction.
     * @since HotA 1.7.0
     */
    class FactoryPatch: public DataPatch
    {
    public:
        void patch(DataPatcher& patcher);
    };
}
