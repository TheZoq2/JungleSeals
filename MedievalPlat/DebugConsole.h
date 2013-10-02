#pragma once

#include "agk.h"
#include "DebugInput.h"

#ifndef DC_GLOBALS
#define DC_GLOBALS
	extern int  dc_textID;
	extern int dc_editID;
	extern int dc_bgSID;

	extern int dc_visible;

	extern uString consoleText;
#endif

class DebugConsole
{
public:
	DebugConsole(void);
	~DebugConsole(void);

	static void setup();

	static void setVisible(int visible);

	static void addToLog(uString msg);
	static void addC(uString msg);

	static bool isActive();
};

