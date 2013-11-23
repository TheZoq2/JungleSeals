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
	registerFunction("getLastActivePart", LUA_getLastActivePart);
	registerFunction("getPartLabelValue", LUA_getPartLabelValue);
}

void LuaHandler::registerFunction(std::string name, lua_CFunction function)
{
	lua_register(lh_state, name.data(), function);
}

void LuaHandler::runScript(std::string name)
{
	luaL_dofile(lh_state, name.data());
}

