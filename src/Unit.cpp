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
}

Unit::Unit(std::fstream& f)
{
	readBinData(f); 
}

bool Unit::readBinData(std::fstream& f)
{
	f.read(reinterpret_cast<char*>(&unitID), 1);
	// Read string size first
	H3Unit::byte stringSize;
	f.read(reinterpret_cast<char*>(&stringSize), 1);
	char* buf = new char[stringSize + 1];
	f.read(buf,stringSize);
	// terminate string
	buf[stringSize] = '\0';
	name = std::string(buf);
	delete[] buf;
	H3Unit::byte town;
	f.read(reinterpret_cast<char*>(&town), 1);
	// cast FactionID to Faction Enum if valid
	if (town <= 10)
		unitFaction = static_cast<Faction>(town);
	else
		unitFaction = Faction::Neutral;
	f.read(reinterpret_cast<char*>(&level), 1);
	f.read(reinterpret_cast<char*>(&attack), 1);
	f.read(reinterpret_cast<char*>(&ammo), 1);
	f.read(reinterpret_cast<char*>(&defense), 1);
	f.read(reinterpret_cast<char*>(&minDmg), 1);
	f.read(reinterpret_cast<char*>(&maxDmg), 1);
	f.read(reinterpret_cast<char*>(&hp), sizeof(ushort));
	f.read(reinterpret_cast<char*>(&speed), 1);
	return f.good();
}

bool Unit::isRanged()
{
	return ammo > 0;
}

bool Unit::isNull()
{
	// Valid unit can't have 0 HP
	return hp == 0;
}

bool Unit::isNeutral()
{
	return unitFaction==Faction::Neutral;
}

H3Unit::byte Unit::getUnitID()
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

H3Unit::byte Unit::getFactionID(Faction whichFaction)
{
	return static_cast<std::underlying_type<Faction>::type>(whichFaction);
}

H3Unit::byte Unit::getLevel()
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

H3Unit::byte Unit::getAttack()
{
	return attack;
}

H3Unit::byte Unit::getAmmo()
{
	return ammo;
}

H3Unit::byte Unit::getDefense()
{
	return defense;
}

H3Unit::byte Unit::getMinDmg()
{
	return minDmg;
}

H3Unit::byte Unit::getMaxDmg()
{
	return maxDmg;
}

ushort Unit::getHP()
{
	return hp;
}

H3Unit::byte Unit::getSpeed()
{
	return speed;
}