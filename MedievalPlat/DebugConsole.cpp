#include "DebugConsole.h"

int  dc_textID;
int dc_editID;

uString consoleText;

DebugConsole::DebugConsole(void)
{
}


DebugConsole::~DebugConsole(void)
{
}

void DebugConsole::setup()
{
	//Debug textr
	dc_textID = agk::CreateText("Debug console");
	agk::FixTextToScreen(dc_textID, 1);
	agk::SetTextSize(dc_textID, 16);
	

	//Creating an editbox
	dc_editID = agk::CreateEditBox();
	agk::FixEditBoxToScreen(dc_editID, 1);
	agk::SetEditBoxPosition(dc_editID, 0, 250);
	agk::SetEditBoxSize(dc_editID, agk::GetVirtualWidth(), 20.0f);

	//Adding a message to the output
	addToLog("Debug console");

	setVisible(0);
}
void DebugConsole::update(World* world, Player* player)
{
	//Enter key for now
	bool enterkey = false;
	
	if(agk::GetRawKeyPressed(13)) enterkey = true;

	if(enterkey) //Running the script typed in the console
	{
		Script::runFunction(agk::GetEditBoxText(dc_editID), NULL, world, player);

		addC("Running function "); addToLog(agk::GetEditBoxText(dc_editID));
		//Clearing the text from the editbox
	}
}

void DebugConsole::addToLog(uString text)
{
	consoleText.Append(text);
	consoleText.Append("\n");

	//Calculating the new height of the debug log
	agk::SetTextString(dc_textID, consoleText);
	float yPos = 250 - agk::GetTextTotalHeight(dc_textID);
	agk::SetTextPosition(dc_textID, 0, yPos);
}

void DebugConsole::addC(uString msg)
{
	consoleText.Append(msg);

	//Calculating the new height of the debug log
	agk::SetTextString(dc_textID, consoleText);
	float yPos = 250 - agk::GetTextTotalHeight(dc_textID);
	agk::SetTextPosition(dc_textID, 0, yPos);
}

void DebugConsole::setVisible(int visible)
{
	agk::SetTextVisible(dc_textID, visible);
	agk::SetEditBoxVisible(dc_textID, visible);
}