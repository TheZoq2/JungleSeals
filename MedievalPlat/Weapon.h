#pragma once

#include "agk.h"
#include "DataReader.h"
#include "DebugConsole.h"

#ifndef WEAP_CONST
#define WEAP_CONST
	extern char weapPath[128];
#endif

class Weapon
{
public:
	Weapon(void);
	~Weapon(void);

	void loadWeaponByName(uString name);
private:
	uString name;

	int SID;
	int magID;

	float dmg;
	float accuracy;
	
	int magCap;
};

