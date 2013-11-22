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
			if(strcmp(dataType.data(), "Food") == 0)
			{
				std::string value = DataReader::getValue(line).GetStr();

				float food = atof(value.data());
				tempReg.setFood(food);
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

	//Reading factions
	agk::SetCurrentDir("");
	agk::SetCurrentDir(WS_factionPath.data());

	file = agk::GetFirstFile();
	currentFile = file;
	delete[] file;
	while(strcmp(currentFile.data(), "") != 0)
	{
		//Creating a temporary faction
		Faction tempFaction;
		//Opening the file
		int fileID = agk::OpenToRead(currentFile.data());

		while(agk::FileEOF(fileID) == 0)
		{
			char* line = agk::ReadLine(fileID);
			//Reading the datatype
			std::string dataType = DataReader::getType(line);

			//Checking the kind of data
			if(strcmp(dataType.data(), "Name") == 0)
			{
				tempFaction.setName(DataReader::getValue(line).GetStr());
			}
			if(strcmp(dataType.data(), "Taxrate") == 0)
			{
				std::string valStr = DataReader::getValue(line).GetStr();

				tempFaction.setTaxrate(atof(valStr.data()));
			}
			if(strcmp(dataType.data(), "Foodmod") == 0)
			{
				std::string valStr = DataReader::getValue(line).GetStr();

				tempFaction.setFoodMod(atof(valStr.data()));
			}
			if(strcmp(dataType.data(), "Wealthmod") == 0)
			{
				std::string valStr = DataReader::getValue(line).GetStr();

				tempFaction.setWealthMod(float(atof(valStr.data())));
			}
			if(strcmp(dataType.data(), "Wealth") == 0)
			{
				std::string valStr = DataReader::getValue(line).GetStr();

				tempFaction.setWealth(float(atof(valStr.data())));
			}
			delete[] line;
		}

		factions->push_back(tempFaction);

		file = agk::GetNextFile();
		currentFile = file;
		delete[] file;
	}

	agk::SetCurrentDir(oldFolder.data());
}

void WorldSim::simulateOnce()
{
	//Going through all the factions
	for(unsigned int i = 0; i < factions->size(); i++)
	{
		std::vector< Region* >* controlledRegions = new std::vector< Region* >; //Vector containing the
				//Regions owned by this factions
		
		std::string factionName = factions->at(i).getName();
		Faction cFaction = factions->at(i);
		//Going thru all the regions and checking if they are owned by this faction
		for(unsigned int n = 0; n < regions->size(); n++)
		{
			//Getting the name of the factions that owns the region
			std::string value = regions->at(n).getOwner();
			
			if(strcmp(value.data(), factionName.data()) == 0)
			{
				controlledRegions->push_back(&regions->at(n));
			}
		}

		//Going thru the regions that were found
		for(unsigned int n = 0; n < controlledRegions->size(); n++)
		{
			//Simulating those region
			controlledRegions->at(n)->simulateOnce(cFaction.getTaxrate(), cFaction.getFoodMod(), cFaction.getWealthMod());

			//Getting values for the faction from the region
		}

		//Sumarising the data for the faction

		//Removing garbage
		delete controlledRegions;
	}
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
		listHeader->push_back("Food");
		listCellWidth->push_back(100);
		listCellWidth->push_back(100);
		listCellWidth->push_back(75);
		listCellWidth->push_back(74);
		listCellWidth->push_back(75);

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
			listItem->push_back(agk::Str(regions->at(i).getFood(), 0));

			//Adding the region to the list
			uiGroup->addToList("worldSimWindow", "regList", listItem);

			//Clearing garbage
			listItem->clear();
			delete listItem;
		}
	}
}
void WorldSim::viewFactions(UI* uiGroup)
{
	if(uiGroup->getWindowExists("simFactionWindow") == false) //If the window does not existr
	{
		//Create the window
		uiGroup->addWindow("simFactionWindow", "1x1.png", 625, 25, 500, 800);
		uiGroup->setWindowColor("simFactionWindow", 150, 150, 150, 200);
		
		std::vector< float >* listCellWidth = new std::vector< float >;
		std::vector< std::string >* listHeader = new std::vector< std::string >;
		listHeader->push_back("Name");
		listHeader->push_back("Taxrate");
		listHeader->push_back("Foodmod");
		listHeader->push_back("Wealthmod");
		listHeader->push_back("Wealth");
		listCellWidth->push_back(100);
		listCellWidth->push_back(100);
		listCellWidth->push_back(100);
		listCellWidth->push_back(100);
		listCellWidth->push_back(75);

		//Adding the list to the window
		uiGroup->addListToWindow("simFactionWindow", "facList", 5, 5, 490, 790, listCellWidth, listHeader);

		listHeader->clear();
		delete listHeader;

		//Adding the region to the list
		for(unsigned int i = 0; i < factions->size(); i++)
		{
			std::vector< std::string >* listItem = new std::vector< std::string >;

			listItem->push_back(factions->at(i).getName());
			listItem->push_back(agk::Str(factions->at(i).getTaxrate(), 2));
			listItem->push_back(agk::Str(factions->at(i).getFoodMod(), 2));
			listItem->push_back(agk::Str(factions->at(i).getWealthMod(), 2));
			listItem->push_back(agk::Str(factions->at(i).getWealth(), 0));

			uiGroup->addToList("simFactionWindow", "facList", listItem);

			listItem->clear();
			delete listItem;
		}
	}
}
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void Region::simulateOnce(float taxrate, float foodMod, float wealthMod)
{
	float foodPerPop = 1;
	//Calculating the food change
	
	//Calculating how much food each person gets in the region
	float foodPerPerson = food / (population * foodPerPop);
	//Removing the food
	float newFood = food - (foodPerPop * population);

	//Calculating the population
	float newPop = population * foodPerPerson;
	
	//Calculating the food production
	newFood = newFood = population * (wealth / 5000);

	//Calculating the wealth
	float newWealth = wealth + (population * 100);

	//Saving the gathered values
	food = newFood;
	population = newPop;
}

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
void Region::setFood(float food)
{
	this->food = food;
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
float Region::getFood()
{
	return food;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void Faction::setup()
{
	//Initialising arrays
	//regions = new std::vector< std::string >;
}

void Faction::setFoodMod(float foodMod)
{
	this->foodMod = foodMod;
}
void Faction::setName(std::string name)
{
	this->name = name;
}
void Faction::setTaxrate(float taxrate)
{
	this->taxrate = taxrate;
}
void Faction::setWealth(float wealth)
{
	this->wealth = wealth;
}
void Faction::setWealthMod(float wealthMod)
{
	this->wealthMod = wealthMod;
}

std::string Faction::getName()
{
	return name;
}
float Faction::getFoodMod()
{
	return foodMod;
}
float Faction::getTaxrate()
{
	return taxrate;
}
float Faction::getWealth()
{
	return wealth;
}
float Faction::getWealthMod()
{
	return wealthMod;
}