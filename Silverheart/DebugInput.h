#pragma once

#include "World.h"
#include "Player.h"
#include "NPC.h"
#include "Script.h"
#include "agk.h"

class DebugInput
{
public:
	DebugInput(void);
	~DebugInput(void);

	static void update(World* world, Player* player, NPCGroup* npcGroup);
};

