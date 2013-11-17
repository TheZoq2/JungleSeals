#include "WorldSim.h"


//Initialising globals
std::string WS_factionPath = "Data/World/Factions";
std::string WS_regionPath = "Data/World/Regions";

WorldSim::WorldSim(void)
{
}

WorldSim::~WorldSim(void)
{
}

void WorldSim::setup()
{
	factions = new std::vector< Faction >;
	regions = new std::vector< Region >;
}

void WorldSim::load()
{
	//Reading region data
	//Going to the region folder
	std::string oldFolder = agk::GetCurrentDir();

	//Setting the new folder
	agk::SetCurrentDir(WS_regionPath.data());

	char* file = agk::GetFirstFile();
	std::string currentFile = file;
	delete[] file;
	while(strcmp(currentFile.data(), "") != 0) //While there are still files to read
	{
		//Temporary region
		Region tempReg;
		
		//Getting the full path to the file

		//Reading the actual file
		int fileID = agk::OpenToRead(currentFile.data());
		while(agk::FileEOF(fileID) == 0)
		{
			char* line = agk::ReadLine(fileID);

			std::string dataType = DataReader::getType(line);

			//Checking what kind of data this is
			if(strcmp(dataType.data(), "Name") == 0)
			{
				tempReg.setName(DataReader::getValue(line).GetStr());
			}
			if(strcmp(dataType.data(), "Owner") == 0)
			{
				std::string value = DataReader::getValue(line).GetStr();
				tempReg.setOwner(value);
			}
			if(strcmp(dataType.data(), "Population") == 0)
			{
				std::string value = DataReader::getValue(line).GetStr();

				tempReg.setPopulation(atoi(value.data()));
			}
			if(strcmp(dataType.data(), "Wealth") == 0)
			{
				std::string value = DataReader::getValue(line).GetStr();

				tempReg.setWealth(atoi(value.data()));
			}


			delete[] line;
		}

		//Selecting the next file
		file = agk::GetNextFile();
		currentFile = file;
		delete[] file;

		//adding the region to the region vector
		regions->push_back(tempReg);
	}

	agk::SetCurrentDir(oldFolder.data());
}

void WorldSim::viewRegions(UI* uiGroup)
{
	if(uiGroup->getWindowExists("worldSimWindow") == false) //If the window does not existr
	{
		//Create the window
		uiGroup->addWindow("worldSimWindow", "1x1.png", 25, 25, 500, 800);
		uiGroup->setWindowColor("worldSimWindow", 150, 150, 150, 200);
		
		std::vector< float >* listCellWidth = new std::vector< float >;
		std::vector< std::string >* listHeader = new std::vector< std::string >;
		listHeader->push_back("Name");
		listHeader->push_back("Owner");
		listHeader->push_back("Pop");
		listHeader->push_back("Wealth");
		listCellWidth->push_back(100);
		listCellWidth->push_back(100);
		listCellWidth->push_back(75);
		listCellWidth->push_back(74);

		//Adding the list to the window
		uiGroup->addListToWindow("worldSimWindow", "regList", 5, 5, 490, 790, listCellWidth, listHeader);

		listHeader->clear();
		delete listHeader;

		//Adding the region to the list
		for(unsigned int i = 0; i < regions->size(); i++)
		{
			//Creating a vector for adding the values to the list
			std::vector< std::string >* listItem = new std::vector< std::string >;

			//ADding the name to the first cell
			listItem->push_back(regions->at(i).getName());
			listItem->push_back(regions->at(i).getOwner());
			listItem->push_back(agk::Str(regions->at(i).getPopulation(), 0));
			listItem->push_back(agk::Str(regions->at(i).getWealth(), 0));

			//Adding the region to the list
			uiGroup->addToList("worldSimWindow", "regList", listItem);

			//Clearing garbage
			listItem->clear();
			delete listItem;
		}
	}
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void Region::setName(std::string name)
{
	this->name = name;
}
void Region::setOwner(std::string owner)
{
	this->owner = owner;
}
void Region::setPopulation(float population)
{
	this->population = population;
}
void Region::setWealth(float wealth)
{
	this->wealth = wealth;
}

std::string Region::getName()
{
	return name;
}
std::string Region::getOwner()
{
	return owner;
}
float Region::getWealth()
{
	return wealth;
}
float Region::getPopulation()
{
	return population;
}