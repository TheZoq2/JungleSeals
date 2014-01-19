#include "DebugInput.h"
#include "DebugConsole.h"

DebugInput::DebugInput(void)
{
}


DebugInput::~DebugInput(void)
{
}

void DebugInput::update(World* world, Player* player, NPCGroup* npcGroup)
{
	//Enter key for now
	bool enterkey = false;
	
	if(agk::GetRawKeyPressed(13)) enterkey = true;

	if(enterkey) //Running the script typed in the console
	{
		Script::runFunction(agk::GetEditBoxText(dc_editID), NULL, world, player, npcGroup);

		DebugConsole::addC("Running function "); DebugConsole::addToLog(agk::GetEditBoxText(dc_editID));
		//Clearing the text from the editbox
		agk::SetEditBoxText(dc_editID, "");
	}
}