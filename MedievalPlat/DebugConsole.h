#pragma once

#include "Script.h"
#include "World.h"
#include "Player.h"

#include "agk.h"

#ifndef DC_GLOBALS
#define DC_GLOBALS
	extern int  dc_textID;
	extern int dc_editID;

	extern uString consoleText;
#endif

class DebugConsole
{
public:
	DebugConsole(void);
	~DebugConsole(void);

	static void setup();
	static void update(World* world, Player* player, NPCGroup* npcGroup);

	static void setVisible(int visible);

	static void addToLog(uString msg);
	static void addC(uString msg);

};

