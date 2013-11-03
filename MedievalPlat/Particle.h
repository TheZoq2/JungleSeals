#pragma once

#include "agk.h"

#include "DebugConsole.h"
#include "DataReader.h"

#include <vector>

#ifndef P_GLOBALS
#define P_GLOBALS
	extern char p_location[128];
#endif

class Particle
{
public:
	Particle(void);
	~Particle(void);

	void addFromFile(int ID, uString filename, float x, float y);
private:
	int groupID;

	int PID;
	
	float x;
	float y;
};

class ParticleGroup
{
public:
	int addFromFile(uString filename, float x, float y);
private:
	std::vector< Particle >* particles;
}

