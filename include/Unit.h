#pragma once
#include <string>
#include "GameObject.h"

/**
* @brief HoMM unit town.
*/
enum class Faction: int{
	Castle,Rampart,Tower,Inferno,Necropolis,Dungeon,Stronghold,Fortress,Conflux,Cove,Factory,
	// 0xFFFFFFFF in file
	Neutral=-1
};

/**
 * @brief Faction aligment.
 */
enum class Aligment: int{
	Good,Neutral,Evil
};

/**
* @brief Stores data about HoMM unit.
*/
class Unit: public GameObject
{
public:
	std::string name;
	Faction unitFaction;

	Unit();
	Unit(ID unitID, std::string name, Faction faction, int level, int hp, int speed, int attack, int defense, int minDmg, int maxDmg, int ammo, ID upgradeID);

	bool isUpgradeable();
	bool isRanged();
	bool isNull();
	bool isNeutral();
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
	* @brief Converts Faction enum into string containing its name.
	* 
	* @param whichFaction Faction enum to convert.
	* @return Name of given faction.
	*/
	static std::string getFactionString(Faction whichFaction);

	/**
	* @brief Based on faction enum returns native terrain name of that faction.
	* 
	* @param whichFaction Faction enum.
	* @param isHota HotA mod makes some changes in the gameplay.
	* @return Native terrain of given faction.
	*/
	static std::string getFactionTerrain(Faction whichFaction,bool isHota = true);

	/**
	* @return Unit faction ID.
	*/
	int getFactionID();

	/**
	* @brief Transforms given faction enum into its ID value.
	*
	* @param whichFaction
	* @return Value of an enum.
	*/
	static int getFactionID(Faction whichFaction);
	int getLevel();

	/**
	* @brief Converts unit level into string. Where number of '+' (plus sign) 
	* represents how many times unit was upgraded.
	* 
	* @return String representing unit level.
	*/
	std::string getLevelString();

	int getAttack();
	int getDefense();
	int getMinDmg();
	int getMaxDmg();
	int getAmmo();
	int getHP();
	int getSpeed();
	ID getUpgradeID();

	void setName(std::string name);
	void setFaction(Faction faction);
	void setLevel(int level);
	void setAttack(int attack);
	void setDefense(int defense);
	void setMinDmg(int minDmg);
	void setMaxDmg(int maxDmg);
	void setAmmo(int ammo);
	void setHP(int hp);
	void setSpeed(int speed);
	void setUpgradeID(ID upgradeID);

private:

	/**
	* Specifies level of an unit, where:
	* 1-7 - is an base unit
	* 8-15 - is an upgraded unit of level 1-7
	* 16-23 - is an 2nd upgrade of a unit of level 1-7
	*/
	int level;
	int attack, defense, minDmg, maxDmg, ammo, speed;
	int hp;

	ID upgradeID;
};

