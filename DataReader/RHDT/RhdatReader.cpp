#include "RhdatReader.h"

namespace rhdat{

    RhdatReader::RhdatReader(std::filesystem::path& pathToFile): BinaryReader(pathToFile)
    {
        reader.read(reinterpret_cast<char*>(&header),sizeof(RhdatHeader));
        if(header.magicBytes!=RHDAT_MAGIC_BYTES)
            throw IllegalFileExtensionException(RHDAT_MAGIC_BYTES,header.magicBytes);
        reader.read(reinterpret_cast<char*>(&unitDataPtr),sizeof(EntryPtr));
    }

    inline Unit RhdatReader::_readUnit(ID id)
    {
        string name=readString();
        int pluralNameSizeToSkip=readInt();
        movePosBy(pluralNameSizeToSkip);
        Faction faction=static_cast<Faction>(readInt());
        int level=readInt();
        int hp=readInt();
        int speed=readInt();
        int attack=readInt();
        int defense=readInt();
        int minDmg=readInt();
        int maxDmg=readInt();
        int ammo=readInt();
        int upgradeID=readInt();
        return Unit(id,name,faction,level,hp,speed,attack,defense,minDmg,maxDmg,ammo,upgradeID);
    }

    ID RhdatReader::findUnitByID(ID unitID)
    {
        reader.seekg(unitDataPtr.dataPtr);
        int maxRetries=unitID;
        ID id=readInt();
        while(id!=unitID&&maxRetries)
        {
            // Skip unit name
            int stringLength=readInt();
            movePosBy(stringLength);
            // Skip plural name + 10 int fields
            unsigned int toSkip=readInt()+10*sizeof(int);
            movePosBy(toSkip);
            id=readInt();
            maxRetries--;
        }
        if(id!=unitID)
            throw ObjectNotFoundException("Unit",unitID);
        return id;
    }

    ID RhdatReader::findUnitByName(string& unitName)
    {
        reader.seekg(unitDataPtr.dataPtr);
        int maxRetries=unitDataPtr.entryCount;
        ID id=readInt();
        string name=readString();
        string pluralName=readString();
        while(name!=unitName&&pluralName!=unitName&&maxRetries&&!reader.eof())
        {
            // Skip 9 int fields
            unsigned int toSkip=9*sizeof(int);
            movePosBy(toSkip);
            id=readInt();
            name=readString();
            maxRetries--;
        }
        if(name!=unitName&&pluralName!=unitName)
            throw ObjectNotFoundException("Unit",unitName);
        reader.seekg(reader.tellg()-unitName.length());
        return id;
    }

    Unit RhdatReader::readUnit(ID unitID)
    {
        if(unitID>unitDataPtr.entryCount)
            throw std::out_of_range("Unit ID: "+std::to_string(unitID)+" out of range");
        ID id=findUnitByID(unitID);
        return _readUnit(unitID);
    }

    Unit RhdatReader::readUnit(string& unitName){
        ID id=findUnitByName(unitName);
        return _readUnit(id);
    }
}