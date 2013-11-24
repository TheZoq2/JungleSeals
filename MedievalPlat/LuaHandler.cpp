#include "LuaHandler.h"

lua_State* lh_state;

//LUA functions
//Player
Player* l_defaultPlr;
int LUA_playerSpawnAt(lua_State* L)
{
	//Getting the position to spawn the player at
	std::string spawnpoint = lua_tostring(L, 1);

	l_defaultPlr->spawn(spawnpoint.data());

	return true;
}
int LUA_setPlayerWeapon(lua_State* L)
{
	//Getting the name of the weapon
	std::string weapon = lua_tostring(L, 1);

	l_defaultPlr->setCurrentWeaponByName(weapon.data());
	return 0;
}
int LUA_movePlayerToPart(lua_State* L)
{
	//Getting parameters from lua
	std::string partName = lua_tostring(L, 1);

	//Getting the part with that name
	Part* targetPart = l_defaultWorld->getPartFromName(partName.data());

	if(targetPart == NULL) //If the part didnt exist
	{
		DebugConsole::addC("Failed to move player to part: ");DebugConsole::addC(partName.data());
		DebugConsole::addToLog(". Part did not exist");

		return 0;
	}

	l_defaultPlr->setPosition(targetPart->getX(), targetPart->getY());

	return 0;
}

//World
World* l_defaultWorld;
int LUA_getPartLabelValue(lua_State* L)
{
	//Getting the value of the parameters passed
	int partID = lua_tointeger(L, 1);
	int label = lua_tointeger(L, 2);

	//Getting the part
	Part* cPart = l_defaultWorld->getPartFromID(partID);

	std::string labelValue = cPart->getLabel(label).GetStr();

	lua_pushstring(L, labelValue.data());

	return 1;
}
int LUA_getLastActivePart(lua_State* L)
{
	lua_pushinteger(L, l_defaultWorld->getLastActive());
	//lua_pushstring(L, "this is a string");

	return 1;
}
int LUA_movePartToPart(lua_State* L)
{
	//Getting the part and target names
	std::string partName = lua_tostring(L, 1);
	std::string targetName = lua_tostring(L, 2);
	float speed = float(lua_tonumber(L, 3));

	//Getting the parts to move
	Part* cPart = l_defaultWorld->getPartFromName(partName.data());
	Part* targetPart = l_defaultWorld->getPartFromName(targetName.data());

	//Making sure nothing is NULL
	if(cPart == NULL)
	{
		DebugConsole::addC("Could not move part: "); DebugConsole::addC(partName.data());
		DebugConsole::addToLog(" to target, part did not exist");

		return 0;
	}
	if(targetPart == NULL)
	{
		DebugConsole::addC("Could not move part: "); DebugConsole::addC(partName.data());
		DebugConsole::addC(" to target, target: ");DebugConsole::addC(targetName.data());
		DebugConsole::addToLog(" did not exist");

		return 0;
	}

	cPart->addOperation("moveToTarget"); //Add the moving operation
	cPart->setTarget(targetPart->getX(), targetPart->getY());
	cPart->setSpeed(speed);

	//Adding the part to  the list of parts to update
	l_defaultWorld->addPartToUpdate(cPart);

	return 0;
}
int LUA_loadNewLevel(lua_State* L)
{
	//Getting parameters from lua
	
	std::string worldName = lua_tostring(L, 1);
	//std::string spawnName = lua_tostring(L, 2);

	std::string worldFile = "levels/";
	worldFile.append(worldName);
	//Making sure the level exists
	if(agk::GetFileExists(worldFile.data()) == 0)
	{
		DebugConsole::addC("Failed to load level: ");DebugConsole::addC(worldName.data());
		DebugConsole::addC(". File: ");DebugConsole::addC(worldFile.data());DebugConsole::addToLog(" did not exist");

		return 0;
	}
	
	//Removing the old world
	l_defaultWorld->clear();
	l_defaultWorld->load(worldFile.data());

	//Spawning the player at the spawnpoint
	//Finding the spawnpoint
	/*Part* spawnPart = l_defaultWorld->getPartFromName(spawnName);
	
	if(spawnPart != NULL)
	{
		l_defaultPlr->setPosition(spawnPart->getX(), spawPart->getY());
	}
	else
	{
		DebugConsole::addC("Failed to put player at spawnpoint: ");DebugConsole::addC(spawnName);DebugConsole::addC(" did not exist");
		l_defaultPlr->setPosition(0, 0);
		return 0;
	}

	return 0;*/
}

//NPCs
NPCGroup* l_defaultNPCgroup;
int LUA_loadNPC(lua_State* L)
{
	//Getting the parameters
	std::string NPCName = lua_tostring(L, 1);
	std::string partName = lua_tostring(L, 2);

	return 0;
}

//Global functions
int LUA_DebugPrint(lua_State* L)
{
	//Getting the parameter to be printed
	std::string printVal = lua_tostring(L, 1);

	DebugConsole::addToLog(printVal.data());

	return 0;
}

LuaHandler::LuaHandler(void)
{
}


LuaHandler::~LuaHandler(void)
{
}

void LuaHandler::setupLua()
{
	lh_state = luaL_newstate();
	luaL_openlibs(lh_state);

	lua_register(lh_state, "printToConsole", LUA_DebugPrint);
	registerFunction("playerSpawnAt", LUA_playerSpawnAt);
	registerFunction("setPlayerWeapon", LUA_setPlayerWeapon);
	registerFunction("movePlayerToPart", LUA_movePlayerToPart);
	registerFunction("loadNewLevel", LUA_loadNewLevel);

	registerFunction("getLastActivePart", LUA_getLastActivePart);
	registerFunction("getLabelValue", LUA_getPartLabelValue);
	registerFunction("movePartToPart", LUA_movePartToPart);
}

void LuaHandler::registerFunction(std::string name, lua_CFunction function)
{
	lua_register(lh_state, name.data(), function);
}

void LuaHandler::runScript(std::string name)
{
	luaL_dofile(lh_state, name.data());
}

