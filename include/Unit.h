#pragma once
#include <string>
#include <fstream>

namespace H3Unit {
	typedef unsigned char byte;
	typedef unsigned short ushort;
}

using namespace H3Unit;

/**
* HoMM unit town.
*/
enum class Faction: int{
	Neutral,Castle,Rampart,Tower,Inferno,Necropolis,Dungeon,Stronghold,Fortress,Conflux,Cove
};

enum class Aligment: int{
        Good,Neutral,Evil
};

/**
* Stores data about HoMM unit.
*/
class Unit
{
public:
	std::string name;
	Faction unitFaction;

	Unit();
	Unit(std::fstream& f);

	/**
	* Reads unit data from a given file.
	*
	* @param f File from whose to read.
	* @return Success of read.
	*/
	bool readBinData(std::fstream& f);
	bool isRanged();
	bool isNull();
	bool isNeutral();
	H3Unit::byte getUnitID();
	std::string getName();

	/**
	* @return Name of unit faction.
	*/
	std::string getFactionString();

	/**
	* @return Native terrain of unit faction.
	*/
	std::string getFactionTerrain();

	/**
	* Converts Faction enum into string containing its name.
	* 
	* @param whichFaction Faction enum to convert.
	* @return Name of given faction.
	*/
	static std::string getFactionString(Faction whichFaction);

	/**
	* Based on faction enum returns native terrain name of that faction.
	* 
	* @param whichFaction Faction enum.
	* @param isHota HotA mod makes some changes in the gameplay.
	* @return Native terrain of given faction.
	*/
	static std::string getFactionTerrain(Faction whichFaction,bool isHota = true);

	/**
	* Transforms given faction enum into its ID value.
	*
	* @param whichFaction
	* @return Value of an enum.
	*/
	static H3Unit::byte getFactionID(Faction whichFaction);
	H3Unit::byte getLevel();

	/**
	* Converts unit level into string. Where number of '+' (plus sign) represents how many
	* times unit was upgraded.
	* 
	* @return String representing unit level.
	*/
	std::string getLevelString();

	H3Unit::byte getAttack();
	H3Unit::byte getDefense();
	H3Unit::byte getMinDmg();
	H3Unit::byte getMaxDmg();
	H3Unit::byte getAmmo();
	H3Unit::ushort getHP();
	H3Unit::byte getSpeed();

private:
	H3Unit::byte unitID;

	/**
	* Specifies level of an unit, where:
	* 1-7 - is an base unit
	* 8-15 - is an upgraded unit of level 1-7
	* 16-23 - is an 2nd upgrade of a unit of level 1-7
	*/
	int level;
	int attack, defense, minDmg, maxDmg, ammo, speed;
	int hp;
};

