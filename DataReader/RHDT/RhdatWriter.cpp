#include "RhdatWriter.h"

namespace rhdat{

    RhdatWriter::RhdatWriter(std::filesystem::path& pathToFile): pathToFile(pathToFile),lastEntry(nullptr),dataPatcher(entries)
    {
        writer=std::ofstream(pathToFile,std::ios_base::binary);
        writeInt(RHDAT_MAGIC_BYTES);
        writeInt(RHDAT_CURRENT_VERSION);
    }

    RhdatWriter::~RhdatWriter()
    {
        if(writer.is_open())
            writer.close();
    }

    inline void RhdatWriter::writeInt(int data)
    {
        writer.write(reinterpret_cast<char*>(&data),sizeof(data));
    }

    inline void RhdatWriter::writeString(std::string str)
    {
        writeInt(str.size());
        writer.write(str.c_str(),str.size());
    }

    int RhdatWriter::findEntry(EntryType entryType) noexcept
    {
        for(int i=0;i<entries.size();i++)
        {
            if(entries.at(i).entryType==entryType)
                return i;
        }
        return -1;
    }

    void RhdatWriter::writeUnits(std::vector<std::unique_ptr<GameObject>>& units)
    {
        for(int i=0;i<units.size();i++)
        {
            UnitExtended* u=static_cast<UnitExtended*>(units.at(i).get());
            writeInt(u->getID());
            writeString(u->getName());
            writeString(u->getPluralName());
            writeInt(u->getFactionID());
            writeInt(u->getLevel());
            writeInt(u->getHP());
            writeInt(u->getSpeed());
            writeInt(u->getAttack());
            writeInt(u->getDefense());
            writeInt(u->getMinDmg());
            writeInt(u->getMaxDmg());
            writeInt(u->getAmmo());
            writeInt(u->getUpgradeID());
        }
    }

    bool RhdatWriter::makeEntry(EntryType entryType) noexcept
    {
        if(findEntry(entryType)!=-1)
            return false;
        WriteEntry entry(entryType);
        entries.push_back(std::move(entry));
        lastEntry=&entries.back();
        return true;
    }

    void RhdatWriter::addDataToSave(std::unique_ptr<GameObject>& data)
    {
        if(lastEntry==nullptr)
            throw NoEntryException();
        lastEntry->objectsToSave.push_back(std::move(data));
    }

    void RhdatWriter::addDataToSave(std::unique_ptr<GameObject>&  data, EntryType entry)
    {
        int entryIndex=findEntry(entry);
        if(entryIndex==-1)
            throw NoEntryException(entry);
        entries.at(entryIndex).objectsToSave.push_back(std::move(data));
    }

    void RhdatWriter::addPatch(DataPatch& patch)
    {
        patch.patch(dataPatcher);
    }

    void RhdatWriter::writeData()
    {
        // Write entry count
        writeInt(entries.size());
        std::streampos beginOfEntries=writer.tellp();
        for(WriteEntry& e:entries)
        {
            // Write entry
            EntryPtr ptrToSave;
            ptrToSave.entryType=e.entryType;
            ptrToSave.entryCount=e.objectsToSave.size();
            // Set pointer initialy to EOF
            ptrToSave.dataPtr=-1u;
            writer.write(reinterpret_cast<char*>(&ptrToSave),sizeof(ptrToSave));
        }
        for(int i=0;i<entries.size();i++)
        {
            WriteEntry& e=entries.at(i);
            std::streampos currPos=writer.tellp();
            // Set position to entry data ptr
            writer.seekp(beginOfEntries+((i+1)*2*sizeof(int)));
            writeInt(currPos);
            // Return to data pos
            writer.seekp(currPos);
            // Write entry data
            switch (e.entryType)
            {
                case EntryType::Units:
                    writeUnits(e.objectsToSave);
                    break;
            }
        }
        writer.flush();
    }
}