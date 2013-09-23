#pragma once

#ifndef NPC_GLOBALS
#define NPC_GLOBALS
	extern char NPC_path[128];
#endif

#include <vector>

#include "agk.h"
#include "DataReader.h"

class Character
{
public:

private:
	int SID;

	float x;
	float y;
};

class NPC
{
public:
	NPC(void);
	~NPC(void);

	void setup();
	void createFromName(uString name);

private:
	uString name;

	std::vector< uString >* formal;

	Character chr;
};

class NPCGroup
{
public:
	void setup();
	void addNPCFromFile(uString file);
private:
	std::vector< NPC >* npc;
};