#include "HdatReader.h"

namespace hdat{

    HdatReader::HdatReader(std::filesystem::path& pathToData): BinaryReader(pathToData)
    {
        reader.read(reinterpret_cast<char*>(&header),sizeof(header));
        if(header.magicBytes!=HDAT_MAGIC_BYTES)
            throw IllegalFileExtensionException(HDAT_MAGIC_BYTES,header.magicBytes);
        _createResourceEntries();
        currObjectNumber=0;
    }

    inline void HdatReader::_createResourceEntries()
    {
        unsigned int entries=0;
        ResourceDescriptor descriptor=readResourceDescriptor();
        bool equalResourceType=true;
        while(equalResourceType)
        {
            entries++;
            _skipUnitData();
            try{
                equalResourceType=(readResourceDescriptor().getResourceType()==descriptor.getResourceType());
                currObjectNumber++;
            }catch(const std::invalid_argument& e){
                equalResourceType=false;
                reader.clear();
            }
        }
        reader.seekg(sizeof(header));
        unitsBegin=ResourceBeginEntry(reader.tellg(),0,entries,MIN_UNIT_ID);
    }

    inline void HdatReader::_skipUnitData()
    {
        // pathLength + 2 ints
        int pathToSkip=readInt()+8;
        skipPosBy(pathToSkip);
        int keyToSkip=readInt();
        skipPosBy(keyToSkip);
        int defToSkip=readInt();
        skipPosBy(defToSkip);
        int nameToSkip=readInt();
        skipPosBy(nameToSkip);
        // Skip plural name + 39 unit data ints + 1 byte
        int toSkip=readInt()+39*4+1;
        skipPosBy(toSkip);
    }

    void inline HdatReader::_skipUnitDataByName()
    {
        int pathToSkip=readInt()+8;
        skipPosBy(pathToSkip);
        int keyToSkip=readInt();
        skipPosBy(keyToSkip);
        int defToSkip=readInt();
        skipPosBy(defToSkip);
    }

    Unit HdatReader::_assignUnitData(ID unitID, string name)
    {
        Faction faction=static_cast<Faction>(readInt());
        int level=readInt()+1;
        skipPosBy(17*4);
        int hp=readInt();
        int speed=readInt();
        int attack=readInt();
        int def=readInt();
        int minDmg=readInt();
        int maxDmg=readInt();
        int ammo=readInt();
        skipPosBy(7*4);
        int upgradeID=readInt();
        return Unit(unitID,name,faction,level,hp,speed,attack,def,minDmg,maxDmg,ammo,upgradeID);
    }

    inline void HdatReader::skipPosBy(std::streampos toSkip)
    {
        reader.seekg(reader.tellg()+toSkip);
    }

    inline void HdatReader::goToResourceBegin(ResourceBeginEntry& resourceToGoTo)
    {
        reader.seekg(resourceToGoTo.getBeginPos());
        currObjectNumber=resourceToGoTo.getResourceNumber();
    }

    ResourceDescriptor HdatReader::findResource(ResourceDescriptor e,ResourceBeginEntry& resEntry)
    {
        goToResourceBegin(resEntry);
        int maxRetries=resEntry.getResourceNumber()+resEntry.getResourceCount();
        ResourceDescriptor currentDesc=readResourceDescriptor();
        while(currObjectNumber<maxRetries&&currentDesc.getResourceID()!=e.getResourceID())
        {
            switch (e.getResourceType())
            {
                case ResourceType::Monster:
                    _skipUnitData();
                    break;
                default:
                    throw std::invalid_argument("Unknown resource");
            }
            currObjectNumber++;
            currentDesc=readResourceDescriptor();
        }
        if(e!=currentDesc)
            throw ObjectNotFoundException(e.getResourceID());
        return currentDesc;
    }

    ResourceDescriptor HdatReader::readResourceDescriptor()
    {
        std::streampos pos=reader.tellg();
        string resourceString=readString();
        size_t idStartPos=resourceString.length()-1;
        while(isdigit(resourceString[idStartPos]))
            idStartPos--;
        // Inc pos to match substr
        idStartPos++;
        string idStr=resourceString.substr(idStartPos);
        string resourceName=resourceString.substr(0,idStartPos);
        return ResourceDescriptor(resourceName,std::stoi(idStr));
    }

    Unit HdatReader::readUnit(ID unitID)
    {
        if(unitID<MIN_UNIT_ID||unitID>unitsBegin.getResourceMaxID())
            throw ObjectNotFoundException("Unit",unitID);
        goToResourceBegin(unitsBegin);
        findResource(ResourceDescriptor(ResourceType::Monster,unitID),unitsBegin);
        // Path + 2 ints
        _skipUnitDataByName();
        string name=readString();
        // Plural + unknown data
        int toSkip=readInt()+4*5+1;
        skipPosBy(toSkip);
        return _assignUnitData(unitID,name);
    }

    Unit HdatReader::readUnit(string &unitName)
    {
        goToResourceBegin(unitsBegin);
        ResourceDescriptor currentDesc;
        string name;
        int maxRetries=unitsBegin.getResourceCount();
        do{
            currentDesc=readResourceDescriptor();
            _skipUnitDataByName();
            name=readString();
            int toSkip=readInt()+39*4+1;
            skipPosBy(toSkip);
            currObjectNumber++;
        }while(currObjectNumber<=maxRetries&&name!=unitName);
        if(name!=unitName)
            throw ObjectNotFoundException("Unit",name);
        return _assignUnitData(currentDesc.getResourceID(),name);
    }
}