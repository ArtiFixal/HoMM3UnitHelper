#include "Unit.h"

Unit::Unit():Unit(0,"",Faction::Neutral,0,0,0,0,0,0,0,0,0) {}

Unit::Unit(ID unitID, std::string name, Faction faction, int level, int hp, int speed, int attack, int defense,
	int minDmg, int maxDmg, int ammo, ID upgradeID): GameObject(unitID),name(name),unitFaction(faction),
	level(level),hp(hp),speed(speed),attack(attack),defense(defense),minDmg(minDmg),maxDmg(maxDmg),
	ammo(ammo),upgradeID(upgradeID) {}

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
	case Faction::Factory:
		return "Factory";
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
	case Faction::Factory:
		return "Wasteland";
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

void Unit::setName(std::string name)
{
	this->name=name;
}

void Unit::setFaction(Faction faction)
{
	unitFaction=faction;
}

void Unit::setLevel(int level)
{
	this->level=level;
}

void Unit::setAttack(int attack)
{
	this->attack=attack;
}

void Unit::setDefense(int defense)
{
	this->defense=defense;
}

void Unit::setMinDmg(int minDmg)
{
	this->minDmg=minDmg;
}

void Unit::setMaxDmg(int maxDmg)
{
	this->maxDmg=maxDmg;
}

void Unit::setAmmo(int ammo)
{
	this->ammo=ammo;
}

void Unit::setHP(int hp)
{
	this->hp=hp;
}

void Unit::setSpeed(int speed)
{
	this->speed=speed;
}

void Unit::setUpgradeID(ID upgradeID)
{
	this->upgradeID=upgradeID;
}
