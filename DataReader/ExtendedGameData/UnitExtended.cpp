#include "UnitExtended.h"

UnitExtended::UnitExtended(ID unitID, std::string name, std::string pluralName, Faction faction, int level, int hp, int speed, int attack, int defense, int minDmg, int maxDmg, int ammo, ID upgradeID):Unit(unitID,name,faction,level,hp,speed,attack,defense,minDmg,maxDmg,ammo,upgradeID),pluralName(pluralName){}

std::string UnitExtended::getPluralName()
{
    return pluralName;
}
