#include "Weapon.h"

char weapPath[128] = "Data/Weapons/";

Weapon::Weapon(void)
{
}


Weapon::~Weapon(void)
{
}

/*void Weapon::loadWeaponByName(uString name)
{
	uString filename;
	filename.SetStr(weapPath);
	filename.Append(name);
	//Making sure the file exists
	if(agk::GetFileExists(filename))
	{

	}
	else
	{
		DebugConsole::addC("Failed to load weapon: ");
		DebugConsole::addC(name);
		DebugConsole::addC(" -- file: ");
		DebugConsole::addC(filename);
		DebugConsole::addToLog(" did not exist");
	}
}*/