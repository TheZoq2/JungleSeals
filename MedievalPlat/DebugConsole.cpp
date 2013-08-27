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
	agk::SetEditBoxSize(dc_editID, agk::GetVirtualWidth(), 20);

	//Adding a message to the output
	addToLog("Debug console");

	setVisible(0);
}

void DebugConsole::addToLog(uString text)
{
	consoleText.Append(text);
	consoleText.Append("\n");

	//Calculating the new height of the debug log
	float yPos = 250 - agk::GetTextTotalHeight(dc_textID);
	agk::SetTextPosition(dc_textID, 0, yPos);
}

void DebugConsole::setVisible(int visible)
{
	agk::SetTextVisible(dc_textID, visible);
	agk::SetEditBoxVisible(dc_textID, visible);
}