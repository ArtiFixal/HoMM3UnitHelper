#include "DataReader.h"

const std::string DataReader::FILE_WITH_DATA="heroesData.rhdat";

const std::string DataReader::H3_BASE_UNIT_FILE="CRTRAITS.TXT";

DataReader::DataReader(std::filesystem::path& pathToMainLodFile,std::filesystem::path& pathToHotaData): hotaDataReader(pathToHotaData)
{
	using namespace h3lod;
	LodReader baseDataReader(pathToMainLodFile);
	// Since we are extracting LOD files anyway lets organize them in more friendly shape.
	std::unique_ptr<GameDataReader<Unit>> unitReader=baseDataReader.getFileReader<Unit>(H3_BASE_UNIT_FILE);
	std::filesystem::path baseGameDataFile=std::filesystem::current_path().concat("\\"+FILE_WITH_DATA);
	if(!std::filesystem::exists(baseGameDataFile))
	{
		RhdatWriter rhdatWriter(baseGameDataFile);
		rhdatWriter.makeEntry(EntryType::Units);
		while(!unitReader->isEof())
		{
			try{
				std::unique_ptr<GameObject> u=unitReader->readData();
				Unit* d=static_cast<Unit*>(u.get());
				if(!d->isNull())
					rhdatWriter.addDataToSave(u);
			}catch(const std::invalid_argument& e){
				// Ignore invalid and empty data
			}
		}
		rhdatWriter.writeData();
	}
	reader=std::make_unique<RhdatReader>(baseGameDataFile);
	lastID = 0;
}

Unit DataReader::readUnit(ID unitID)
{
	try{
		if(unitID<150)
			return reader->readUnit(unitID);
		return hotaDataReader.readUnit(unitID);
	}catch(const ObjectNotFoundException& e){
		return Unit();
	}
}