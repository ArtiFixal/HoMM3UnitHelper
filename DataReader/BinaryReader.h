#pragma once
#include <fstream>
#include <filesystem>

/**
 * @brief Base class for the readers reading binary data.
 */
class BinaryReader
{
protected:
    std::filesystem::path& pathToFile;
    std::ifstream reader;

	/**
	 * @brief Moves current read position by given value.
     * 
	 * @param toMove How many bytes to move. Positive value will 
     * move forward, negative backward.
	 */
	inline void movePosBy(int toMove){
        reader.seekg(reader.tellg()+toMove);
    }

public:
    BinaryReader(std::filesystem::path& pathToFile);
    ~BinaryReader();

    inline int readInt(){
        int data;
        reader.read(reinterpret_cast<char*>(&data),sizeof(data));
        return data;
    }

    inline std::string readString(){
        int stringLength=readInt();
        char* buff=new char[stringLength];
        reader.read(buff,stringLength);
        return std::string(buff,stringLength);
    }

    std::filesystem::path& getPathToFile();
};