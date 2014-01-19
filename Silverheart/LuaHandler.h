#pragma once

#include <string>

#include "LUA/lua.hpp"

#include "DebugConsole.h"
#include "Player.h"
//#include "World.h"
#include "NPC.h"

typedef int (*lua_CFunction) (lua_State *L);

#ifndef LH_LUAGLOBALS
#define LH_LUAGLOBALS
	extern lua_State* lh_state;

	extern int LUA_DebugPrint(lua_State* L);

	extern Player* l_defaultPlr;
	extern int LUA_playerSpawnAt(lua_State* L);
	extern int LUA_setPlayerWeapon(lua_State* L);
	extern int LUA_movePlayerToPart(lua_State* L);

	extern World* l_defaultWorld;
	extern int LUA_getPartLabelValue(lua_State* L); //Gets the value of the label for a part with the name and label ID passed
	extern int LUA_getLastActivePart(lua_State* L);
	extern int LUA_movePartToPart(lua_State* L);

	extern NPCGroup* l_defaultNPCgroup;
	extern int LUA_loadNPC(lua_State* L);
#endif

class LuaHandler
{
public:
	LuaHandler(void);
	~LuaHandler(void);

	static void setupLua();

	static void registerFunction(std::string name, lua_CFunction function);
	static void printError(lua_State* L, char* msg);

	static void runScript(std::string name);
};