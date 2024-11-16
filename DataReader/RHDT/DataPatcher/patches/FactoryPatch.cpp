#include "FactoryPatch.h"
#include "Unit.h"

namespace rhdat
{
    void FactoryPatch::patch(DataPatcher& patcher)
    {
        constexpr ID halflingID=138;
        Unit* halfling=reinterpret_cast<Unit*>(patcher.getObject(EntryType::Units,halflingID).get());
        halfling->setFaction(Faction::Factory);
        constexpr ID halflingGranadierID=171;
        halfling->setUpgradeID(halflingGranadierID);
    }
}