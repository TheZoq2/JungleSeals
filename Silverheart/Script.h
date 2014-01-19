#pragma once

#include "agk.h"
#include "World.h"
#include "Player.h"
#include "Part.h"
#include "IngameMenu.h"
#include "NPC.h"

#include <vector>

class Script
{
public:
	Script(void);
	~Script(void);

	static void run(uString scriptName, Part* part, World* world, Player* player); //Scriptname - Script to run, Part - The part that started the script, Pointers - what should be affected by the script

	static void runFunction(uString function, Part* part, World* world, Player* player, NPCGroup* npcGroup);
	static void runFunction(uString function, Part* part, World* world, Player* player);

	static bool isSpecial(uString param);
	static bool isLabel(uString param); //Will check if the parameter is a label
	static uString getValueFromLabel(uString param, Part* part);

	static uString getParam(uString cmd, int number, bool isLast, Part* part);
};

