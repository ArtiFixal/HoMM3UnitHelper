#include "DataReader.h"

const std::string DataReader::FILE_WITH_DATA="unitData.hdat";

DataReader::DataReader()
{
	unitData = fstream(FILE_WITH_DATA, ios_base::in | ios_base::binary);
	lastID = 0;
}

DataReader::DataReader(string unitDataFileName)
{
	unitData = fstream(unitDataFileName, ios_base::in | ios_base::binary);
	lastID = 0;
}

DataReader::~DataReader()
{
	if (unitData.is_open())
	{
		unitData.close();
	}
}

Unit DataReader::readUnit(unsigned char unitID)
{
	Unit u;
	// In game exists only 160 units, where max id is 170
	if (unitID > 170)
		return u;
	if (unitData.eof())
	{
		unitData.clear();
		unitData.seekg(0, ios_base::beg);
	}
	H3Unit::byte id;
	// Reposition file offset to file begin if needed
	if (lastID>unitID)
		unitData.seekg(0, ios_base::beg);
	while (!unitData.eof())
	{
		unitData.read(reinterpret_cast<char*>(&id), 1);
		if (unitData.good())
		{
			if (id == unitID)
			{
				unitData.seekg(-1, ios_base::cur);
				u.readBinData(unitData);
				lastID = id;
				break;
			}
			H3Unit::byte stringLength;
			unitData.read(reinterpret_cast<char*>(&stringLength), 1);
			// Skip rest of unit data:
			// 8B (uchar) + 2B (ushort) after unit ID and name
			unitData.seekg((streamoff)stringLength + 10, ios_base::cur);
		}
	}
	return u;
}

bool DataReader::reloadFile()
{
	if (unitData.is_open())
		unitData.close();
	unitData.open(FILE_WITH_DATA, ios_base::in | ios_base::binary);
	lastID = 0;
	return unitData.good();
}