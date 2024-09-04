#pragma once
#include <fstream>
#include "Unit.h"

using namespace std;

/**
* Reads unit/game data.
*/
class DataReader
{
public:
	// Path to a file containing unit data
	static const std::string FILE_WITH_DATA;

	DataReader();
	DataReader(string unitDataFileName);
	~DataReader();

	/**
	* Reads external file in search for given unit data.
	* 
	* @param unitID Which unit data we want to read.
	*/
	Unit readUnit(unsigned char unitID);
	bool reloadFile();

private:
	// File containing unit data
	fstream unitData;

	// Last read unit ID
	H3Unit::byte lastID;
};

