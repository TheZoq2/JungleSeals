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
private:
	std::string name;
};

class Region
{
public:
	void setName(std::string name);
	void setOwner(std::string owner);
	void setPopulation(float population);
	void setWealth(float wealth);

	std::string getName();
	std::string getOwner();
	float getPopulation();
	float getWealth();
private:
	std::string name;
	
	std::string owner;
	
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
private:
	std::vector< Region >* regions;
	std::vector< Faction >* factions;
};