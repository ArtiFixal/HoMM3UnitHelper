#pragma once
#include <fstream>
#include <memory>
#include "UnitExtended.h"
#include "LodReader.h"
#include "UnitDataReader.h"
#include "HdatReader.h"
#include "RhdatWriter.h"
#include "RhdatReader.h"
#include "FactoryPatch.h"

using namespace rhdat;
using namespace hdat;

/**
* @brief Reads game data.
*/
class DataReader
{
private:
	std::unique_ptr<RhdatReader> reader;
	HdatReader hotaDataReader;

	// Last read unit ID
	int lastID;

public:
	// Path to a file containing data
	static const string FILE_WITH_DATA;

	/**
     * Where unit data is stored in LOD file.
     */
	static const string H3_BASE_UNIT_FILE;
	
	/**
	 * @param pathToMainLodFile LOD file containing all data.
	 */
	DataReader(std::filesystem::path& pathToMainLodFile,std::filesystem::path& pathToHotaData);
	~DataReader(){};

	/**
	* @brief Reads external file in search for given unit data.
	* 
	* @param unitID Which unit data we want to read.
	*/
	Unit readUnit(ID unitID);

	/**
	 * @brief Reads external file in search for given unit data.
	 * 
	 * @param unitName Which unit data we want to read.
	 * @return Unit with given name.
	 */
	Unit readUnit(string& unitName);
};
