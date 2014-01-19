#pragma once

#include <vector>
#include <string>

#include "agk.h"

#include "DebugConsole.h"
#include "DataReader.h"
#include "UI.h"

#ifndef WS_GLOBALS
#define WS_GLOBALS
	extern std::string WS_factionPath;
	extern std::string WS_regionPath;
#endif

class Faction
{
public:
	void setup();

	void setName(std::string name);
	void setTaxrate(float taxrate);
	void setFoodMod(float foodMod);
	void setWealthMod(float wealthMod);
	void setWealth(float wealth);

	std::string getName();
	float getTaxrate();
	float getFoodMod();
	float getWealthMod();
	float getWealth();
private:
	std::string name;

	float taxrate;
	float foodMod;
	float wealthMod;
	float wealth;

	//std::vector< std::string >* regions;
};

class Region
{
public:
	//Simulate the region one cycle based on the stats of the faction controlling the region
	void simulateOnce(float taxrate, float foodMod, float wealthMod); 

	void setName(std::string name);
	void setOwner(std::string owner);
	void setPopulation(float population);
	void setWealth(float wealth);
	void setFood(float food);

	std::string getName();
	std::string getOwner();
	float getPopulation();
	float getWealth();
	float getFood();
private:
	std::string name;
	
	std::string owner;
	
	float food;
	float population;
	float wealth;
};

class WorldSim
{
public:
	WorldSim(void);
	~WorldSim(void);

	void setup();

	void load();

	void viewRegions(UI* uiGroup);
	void viewFactions(UI* uiGroup);

	void simulateOnce();
private:
	std::vector< Region >* regions;
	std::vector< Faction >* factions;
};