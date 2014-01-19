#include "GF.h"

//Defining static variables
char GF::sdPath[128] = "media/";
int GF::quality = 1;
int GF_BaseDepth = 100;
int GF_charGroup = 2;

GF::GF(void)
{
}

GF::~GF(void)
{
}

float globaltime = 0;
float oldTime = 0;
float frameTime = 0;
float speedMod = 0;
float physScale = 0;

void GF::setupGlobals()
{
	globaltime = 0;
	oldTime = 0;
	frameTime = 0;
	speedMod = 0;
}

uString GF::getPath(char originalPath[128])
{
	uString result;
	
	if(GF::quality == 1)
	{
		result.SetStr(sdPath);
		result.Append(originalPath);
	}

	return result; //Returning the final path
}
uString GF::getPath(uString filename)
{
	uString result;

	if(GF::quality == 1)
	{
		result.SetStr(sdPath);
		result.Append(filename);
	}

	return result;
}

void GF::updateTime()
{
	oldTime = globaltime; //Saving the old time

	globaltime = agk::Timer(); //Getting the timer
	
	frameTime = globaltime - oldTime; //Calculating the frame time

	speedMod = 60*frameTime;
}

void GF::changeState(bool* variable)
{
	if(*variable == true)
	{
		*variable = false;
	}
	else
	{
		*variable = true;
	}
}