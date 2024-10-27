#include "Unit.h"

Unit::Unit() {
	unitID = 0;
	level = 0;
	unitFaction = Faction::Neutral;
	attack = 0;
	defense = 0;
	minDmg = 0;
	maxDmg = 0;
	ammo = 0;
	speed = 0;
	hp = 0;
	upgradeID = 0;
}

Unit::Unit(ID unitID, std::string name, Faction faction, int level, int hp, int speed, int attack, int defense, int minDmg, int maxDmg, int ammo, ID upgradeID)
{
	this->unitID = unitID;
	this->name=name;
	this->unitFaction = faction;
	this->level = level;
	this->hp = hp;
	this->speed = speed;
	this->attack = attack;
	this->defense = defense;
	this->minDmg = minDmg;
	this->maxDmg = maxDmg;
	this->ammo = ammo;
	this->upgradeID=upgradeID;
}

bool Unit::isUpgradeable()
{
    return upgradeID!=-1;
}

bool Unit::isRanged()
{
	return ammo > 0;
}

bool Unit::isNull()
{
	// Valid unit can't have <= 0 HP
	return hp <= 0;
}

bool Unit::isNeutral()
{
	return unitFaction==Faction::Neutral;
}

ID Unit::getUnitID()
{
	return unitID;
}

std::string Unit::getName()
{
	return name;
}

std::string Unit::getFactionString()
{
	return getFactionString(unitFaction);
}

std::string Unit::getFactionTerrain()
{
	return getFactionTerrain(unitFaction);
}

std::string Unit::getFactionString(Faction whichFaction)
{
	switch (whichFaction)
	{
	case Faction::Neutral:
		return "Neutral";
	case Faction::Castle:
		return "Castle";
	case Faction::Rampart:
		return "Rampart";
	case Faction::Tower:
		return "Tower";
	case Faction::Inferno:
		return "Inferno";
	case Faction::Necropolis:
		return "Necropolis";
	case Faction::Dungeon:
		return "Dungeon";
	case Faction::Stronghold:
		return "Stronghold";
	case Faction::Fortress:
		return "Fortress";
	case Faction::Conflux:
		return "Conflux";
	case Faction::Cove:
		return "Cove";
	default:
		return "ERROR";
	}
}

std::string Unit::getFactionTerrain(Faction whichFaction,bool isHota)
{
	switch (whichFaction)
	{
	case Faction::Neutral:
		return "None";
	case Faction::Castle:
	case Faction::Rampart:
		return "Grass";
	case Faction::Tower:
		return "Snow";
	case Faction::Inferno:
		return "Lava";
	case Faction::Necropolis:
		return "Dirt";
	case Faction::Dungeon:
		return "Subterranean";
	case Faction::Stronghold:
		return "Rough";
	case Faction::Fortress:
	case Faction::Cove:
		return "Swamp";
	case Faction::Conflux:
		return (isHota)?"Highlands":"Grass";
	default:
		return "ERROR";
	}
}

int Unit::getFactionID(){
	return getFactionID(unitFaction);
}

int Unit::getFactionID(Faction whichFaction)
{
	return static_cast<std::underlying_type<Faction>::type>(whichFaction);
}

int Unit::getLevel()
{
	return level;
}

std::string Unit::getLevelString()
{
	if (level >= 16)
		return std::to_string(level - 15) + "++";
	else if (level >= 8)
		return std::to_string(level - 7) + "+";
	else
		return std::to_string(level);
}

int Unit::getAttack()
{
	return attack;
}

int Unit::getAmmo()
{
	return ammo;
}

int Unit::getDefense()
{
	return defense;
}

int Unit::getMinDmg()
{
	return minDmg;
}

int Unit::getMaxDmg()
{
	return maxDmg;
}

int Unit::getHP()
{
	return hp;
}

int Unit::getSpeed()
{
	return speed;
}

ID Unit::getUpgradeID()
{
    return upgradeID;
}
