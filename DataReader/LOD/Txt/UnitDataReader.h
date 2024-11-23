#pragma once
#include "h3lod_defs.h"
#include "TxtReader.h"
#include "UnitExtended.h"

#define UNIT_FIGHT_VALUE_POS 9
#define UNIT_STATS_POS 13

namespace h3lod{

    /**
     * @brief Reads @code Unit data from TXT files inside .LOD archive.
     */
    class UnitDataReader: public TxtReader<UnitExtended>
    {
        private:

            /**
             * @brief Calculates upgrade ID for given unit. Viable for 
             * all factions except Conflux and Neutral.
             * 
             * @param unitID For who to calc.
             * 
             * @return Unit upgrade ID, or -1 if Unit is not upgreadable.
             */
            uint _calcUnitUpgradeID(uint unitID) noexcept;

            /**
             * @brief Calculates Unit level for given unit.
             * 
             * @param unitID For who to calc.
             * 
             * @return Unit level.
             */
            uint _calcUnitLevel(uint unitID) noexcept;

            /**
             * @brief Reads data from file and assigns it to the pointer.
             * 
             * @param strings Strings read from a file signle line.
             * 
             * @return Read unit.
             * 
             * @throws std::invalid_argument If failed to parse line of data.
             */
            std::unique_ptr<UnitExtended> assignUnitData(std::vector<string>& strings);

            void _setConfluxData(uint& unitID,uint& upgradeID,Faction& faction,int& levelRef,int levelVal) noexcept;
            void _setElementalData(uint& unitID,Faction& faction,uint& upgradeID ,uint movedBy,int& levelRef,int levelVal) noexcept;
            int _assignUnitLevelNeutral(uint unitID) noexcept;

        public:
            UnitDataReader(LodFile& fileToRead);
            ~UnitDataReader(){};
            
            std::unique_ptr<UnitExtended> readData() override;
            std::unique_ptr<UnitExtended> readData(uint unitID) override;
            std::unique_ptr<UnitExtended> readData(string unitName) override;
    };
}