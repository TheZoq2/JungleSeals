#pragma once

#include "agk.h"
#include "DataReader.h"
#include "DebugConsole.h"
#include "GF.h"

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

	void setPosition(float x, float y);

	void targetPos(float targetX, float targetY);
private:
	uString name;

	bool exists;

	int imgID;
	int SID;
	int magID;

	float spread;
	
	int magCap;

	float offsetX;
	float offsetY;

	float x;
	float y;

	float scaleX;
	float scaleY;

	float angle;
};

