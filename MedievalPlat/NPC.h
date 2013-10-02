#pragma once

#ifndef NPC_GLOBALS
#define NPC_GLOBALS
	extern char NPC_path[128];
#endif

#include <vector>

#include "agk.h"
#include "DataReader.h"
#include "World.h"
#include "GF.h"
#include "Player.h"

class Character
{
public:
	void create(uString colSprite);
	void update(World* world);

	bool checkOnGround(World* world);

	//These functions should be called if the character should do something
	void jump();

	void setPosition(float x, float y);
	
	float getX();
	float getY();
private:
	int SID;
	int imgID;

	float x;
	float y;

	float colScale;

	float lastJump;
	float jumpHeight;
	bool isOnGround;
};

class NPC
{
public:
	NPC(void);
	~NPC(void);

	void setup();
	void update(World* world);
	void updateChars(std::vector< NPC >* npc, Player* player);
	void createFromName(uString name);

	void setPosition(float x, float y);

private:
	bool exists;

	uString name;

	std::vector< uString >* formal;

	Character chr;

	float x;
	float y;

	int state; //The current state of the NPC
};

class NPCGroup
{
public:
	void setup();
	void update(World* world); //Main update loop for NPCs
	void updateChars(NPCGroup* npcGroup, Player* player);

	void addNPCFromFile(uString file);
	void addNPCFromFile(uString file, float x, float y);
private:
	std::vector< NPC >* npc;
};