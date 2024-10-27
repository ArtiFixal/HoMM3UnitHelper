#pragma once
#include <filesystem>
#include <fstream>
#include <vector>
#include <memory>
#include "Unit.h"
#include "Rhdat.h"
#include "NoEntryException.h"

namespace rhdat{

    /**
     * @brief Saves data in RHDAT format.
     */
    class RhdatWriter
    {
    private:
        struct WriteEntry
        {
            WriteEntry(EntryType type):entryType(type){}
            WriteEntry(const WriteEntry&)=delete;
            WriteEntry(WriteEntry&&)=default;
            EntryType entryType;
            std::vector<std::unique_ptr<GameObject>> objectsToSave;
        };

        std::filesystem::path& pathToFile;
        std::ofstream writer;

        /**
         * @brief Entries to save.
         */
        std::vector<WriteEntry> entries;

        /**
         * @brief Last created entry.
         */
        WriteEntry* lastEntry;

        void writeInt(int data);
        void writeString(string str);
        void writeUnits(std::vector<std::unique_ptr<GameObject>>& units);

        /**
         * @brief Searches for given entry type.
         * 
         * @param entryType What to look for.
         * 
         * @return If exists entry index, -1 otherwise.
         */
        int findEntry(EntryType entryType) noexcept;

    public:
        /**
         * @param pathToFile Where to create a file.
         */
        RhdatWriter(std::filesystem::path& pathToFile);
        ~RhdatWriter();

        /**
         * @brief Creates new file entry.
         * 
         * @param entryType Type of entry to create.
         * 
         * @return True if entry was created, false if it already exists.
         */
        bool makeEntry(EntryType entryType) noexcept;

        /**
         * @brief Adds data to be saved into last created entry.
         * 
         * @param data What to save.
         * 
         * @throw NoEntryException If no entry was previously made.
         */
        void addDataToSave(std::unique_ptr<GameObject>& data);

        /**
         * @brief Adds data to be saved into last created entry.
         * 
         * @param data What to save.
         * @param entry Where to save.
         * 
         * @throw NoEntryException If entry was not found.
         */
        void addDataToSave(std::unique_ptr<GameObject>& data,EntryType entry);

        /**
         * @brief Writes prepared data to the file.
         */
        void writeData();
    };

}