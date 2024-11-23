#pragma once
#include "Unit.h"

/**
 * @brief Extended @code Unit data that will be saved to the file.
 */
class UnitExtended: public Unit
{
private:
    std::string pluralName;

public:
    UnitExtended(ID unitID, std::string name,std::string pluralName, Faction faction, int level, int hp, int speed, int attack, int defense, int minDmg, int maxDmg, int ammo, ID upgradeID);
    ~UnitExtended(){};

    std::string getPluralName();
};
