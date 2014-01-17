#include "DebugConsole.h"

int  dc_textID;
int dc_editID;
int dc_bgSID;
int dc_visible = 0;

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
	agk::SetEditBoxSize(dc_editID, (float)agk::GetVirtualWidth(), 20.0f);
	agk::SetEditBoxBorderColor(dc_editID, 150, 150, 150, 150);
	agk::SetEditBoxBackgroundColor(dc_editID, 150, 150, 150, 150);

	//Creating the background sprite
	dc_bgSID = agk::CloneSprite(1);
	agk::FixSpriteToScreen(dc_bgSID, 1);
	agk::SetSpriteScale(dc_bgSID, (float)agk::GetVirtualWidth(), 270.0f);
	agk::SetSpriteColor(dc_bgSID, 150, 150, 150, 100);
	agk::SetSpriteVisible(dc_bgSID, 0);

	//Adding a message to the output
	addToLog("Debug console");

	setVisible(0);
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
	agk::SetEditBoxVisible(dc_editID, visible);
	agk::SetSpriteVisible(dc_bgSID, visible);

	if(visible == 1)
	{
		agk::SetEditBoxFocus(dc_editID, 1);
	}

	dc_visible = visible;
}

bool DebugConsole::isActive()
{
	if(dc_visible == 0)
	{
		return false;
	}
	
	return true;
}