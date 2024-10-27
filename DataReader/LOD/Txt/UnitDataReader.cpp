#include "UnitDataReader.h"

namespace h3lod{

    UnitDataReader::UnitDataReader(LodFile& fileToRead): TxtReader<Unit>(fileToRead){}

    inline uint UnitDataReader::_calcUnitUpgradeID(uint unitID) noexcept
    {
        // Upgraded unit is next to base one
        if(unitID&1)
            return -1;
        else
            return unitID+1;
    }

    uint UnitDataReader::_calcUnitLevel(uint unitID) noexcept
    {
        // Each faction owns 14 units
        int level=(unitID%14)/2+1;
        return level;
    }

    std::unique_ptr<Unit> UnitDataReader::assignUnitData(std::vector<std::string>& strings)
    {
        if(reader.eof()||strings.empty())
            throw std::invalid_argument("Unable to parse Unit data");
        uint unitID=getCurrentLineNum()-1;
        Faction faction;
        uint upgradeID;
        int level;
        // Before air elemental line
        if(unitID<139)
        {
            // After each faction there are 3 blank lines, until elementals...
            // 14 units + 3 lines
            unitID-=((unitID/17)+1)*3;
            // calc Faction ID
            faction=static_cast<Faction>(unitID/14);
            upgradeID=_calcUnitUpgradeID(unitID);
            level=_calcUnitLevel(unitID);
        }
        // Conflux-Neutral mess
        else
        {
            switch (unitID)
            {
                // Elementals
                case 139: // Air
                case 157: // Storm
                    /**
                     * upgradeID = upgradedID - baseID:
                     * storm - air = 15
                     * magma - earth = 12
                     * energy - fire = 15
                     * ice - water = 8
                     */
                    _setElementalData(unitID,faction,upgradeID,15,level,2);
                    break;
                case 140: // Earth
                case 155: // Magma
                    _setElementalData(unitID,faction,upgradeID,12,level,5);
                    break;
                case 141: // Fire
                case 159: // Energy
                    _setElementalData(unitID,faction,upgradeID,15,level,4);
                    break;
                case 142: // Water
                case 153: // Ice
                    _setElementalData(unitID,faction,upgradeID,8,level,3);
                    break;
                // Firebird
                case 160:
                case 161:
                    _setConfluxData(unitID,upgradeID,faction,level,7);
                    break;
                // Psychic elemental
                case 150:
                case 151:
                    _setConfluxData(unitID,upgradeID,faction,level,6);
                    break;
                // Pixie
                case 148:
                case 149:
                    _setConfluxData(unitID,upgradeID,faction,level,1);
                    break;
                default:
                    // Before Conflux units
                    if(unitID<148) 
                        unitID-=9*3;
                    // Siege machines
                    else if(unitID>177)
                        unitID-=12*3;
                    // Neutral
                    else
                        unitID-=11*3;
                    faction=Faction::Neutral;
                    upgradeID=-1;
                    level=_assignUnitLevelNeutral(unitID);
                    break;
            }
        }
        int hp=stoi(strings.at(UNIT_STATS_POS));
        int speed=stoi(strings.at(UNIT_STATS_POS+1));
        int attack=stoi(strings.at(UNIT_STATS_POS+2));
        int defense=stoi(strings.at(UNIT_STATS_POS+3));
        int minDmg=stoi(strings.at(UNIT_STATS_POS+4));
        int maxDmg=stoi(strings.at(UNIT_STATS_POS+5));
        int ammo=stoi(strings.at(UNIT_STATS_POS+6));
        return std::make_unique<Unit>(unitID,strings.at(0),faction,level,hp,speed,attack,defense,minDmg,maxDmg,ammo,upgradeID);
    }

    inline void UnitDataReader::_setElementalData(uint& unitID,Faction& faction,uint& upgradeID ,uint movedBy,int& levelRef,int levelVal) noexcept
    {
        unitID-=9*3;
        faction=Faction::Conflux;
        if(unitID>115)
        {
            unitID-=3;
            upgradeID=-1;
        }
        else
            upgradeID=unitID+movedBy;
        levelRef=levelVal;
    }

    inline void UnitDataReader::_setConfluxData(uint& unitID,uint& upgradeID,Faction& faction,int& levelRef,int levelVal) noexcept
    {
        // 8 factions + elementals
        unitID-=10*3;
        faction=Faction::Conflux;
        levelRef=levelVal;
        upgradeID=_calcUnitUpgradeID(unitID);
    }

    inline int UnitDataReader::_assignUnitLevelNeutral(uint unitID) noexcept
    {
        switch (unitID)
        {
            case 136: // Enchanter
                return 6;
            case 137: // Sharpshooter
                return 4;
            case 138: // Halfling
            case 139: // Peasant
                return 1;
            case 140: // Boar
            case 143: // Rogue
                return 2;
            case 141: // Mummy
            case 142: // Nomad
                return 3;
            case 144: // Troll
                return 5;
            default: // Dragons
                return 7;
        }
    }

    std::unique_ptr<Unit> UnitDataReader::readData()
    {
        std::vector<string> strings=readStrings();
        return assignUnitData(strings);
    }

    std::unique_ptr<Unit> UnitDataReader::readData(uint unitID)
    {
        goToLine(unitID);
        std::vector<string> strings=readStrings();
        return assignUnitData(strings);
    }

    std::unique_ptr<Unit> UnitDataReader::readData(string unitName)
    {
        std::vector<string> strings;
        reader.seekg(fileBegin);
        string name,pluralName;
        reader>>name;
        reader>>pluralName;
        while(!reader.eof()&&name!=unitName&&name!=pluralName)
        {
            skipLine();
            reader>>name;
            reader>>pluralName;
        }
        strings=readStrings('\t');
        strings.insert(strings.begin(),name);
        return assignUnitData(strings);
    }
}