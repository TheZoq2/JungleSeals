#include "Weapon.h"

char weapPath[128] = "Data/Weapons/";

Weapon::Weapon(void)
{
}


Weapon::~Weapon(void)
{
}

void Weapon::loadWeaponByName(uString name)
{
	uString filename;
	filename.SetStr(weapPath);
	filename.Append(name);
	//Making sure the file exists
	if(agk::GetFileExists(filename))
	{
		uString spritePath;
		spritePath.SetStr(" ");
		//Reading the file
		int fileID = agk::OpenToRead(filename);

		while(agk::FileEOF(fileID) == 0) //Looping thru all the lines of the file
		{
			char* line; //Char pointer for storing the lines
			
			line = agk::ReadLine(fileID);

			if(DataReader::getType(line).CompareTo("Sprite") == 0) //Sprite path
			{
				spritePath.SetStr(DataReader::getValue(line));
			}
			if(DataReader::getType(line).CompareTo("Name") == 0) //Weapon name
			{
				name.SetStr(DataReader::getValue(line));
			}
			if(DataReader::getType(line).CompareTo("Spread") == 0) //Spread
			{
				uString spreadStr;
				spreadStr.SetStr(DataReader::getValue(line));

				spread = atof(spreadStr);
			}
			if(DataReader::getType(line).CompareTo("OffsetX") == 0) //OffsetX
			{
				uString offsetXStr;
				offsetXStr.SetStr(DataReader::getValue(line));
				
				offsetX = atof(offsetXStr);
			}
			if(DataReader::getType(line).CompareTo("OffsetY") == 0) //OffsetY
			{
				uString offsetYStr;
				offsetYStr.SetStr(DataReader::getValue(line));
				
				offsetY = atof(offsetYStr);
			}
			if(DataReader::getType(line).CompareTo("ScaleX") == 0) //ScaleX
			{
				uString scaleXStr;
				scaleXStr.SetStr(DataReader::getValue(line));

				scaleX = atof(scaleXStr);
			}
			if(DataReader::getType(line).CompareTo("ScaleY") == 0) //ScaleY
			{
				uString scaleYStr;
				scaleYStr.SetStr(DataReader::getValue(line));

				scaleY = atof(scaleYStr);
			}

			delete[] line; //Deleting the string
		}

		agk::CloseFile(fileID);

		//Making sure a path to the image has been set before loading the weapon
		if(spritePath.CompareTo(" ") != 0)
		{
			//A path has been specified, try loading it
			uString spriteFile;
			spriteFile.SetStr(GF::getPath(spritePath));

			if(agk::GetFileExists(spriteFile))
			{
				//The file existed, let's load it
				imgID = agk::LoadImage(spriteFile);
				SID = agk::CreateSprite(imgID);

				agk::SetSpriteOffset(SID, offsetX, offsetY); //Setting the offset of the weapon sprite
				agk::SetSpriteScale(SID, scaleX, scaleY);
				agk::SetSpriteDepth(SID, 15);
				exists = true;
			}
			else //The file didn't exist, alert the user
			{
				DebugConsole::addC("Image: ");
				DebugConsole::addC(spriteFile);
				DebugConsole::addC(" was not found when loading weapon: ");
				DebugConsole::addC(name);
				DebugConsole::addC(" -- ");
				DebugConsole::addToLog(filename);
			}
		}
		else
		{
			DebugConsole::addC("No spritename was set when loading weapon: ");
			DebugConsole::addC(name);
			DebugConsole::addC(" -- ");
			DebugConsole::addToLog(filename);
		}
	}
	else
	{
		DebugConsole::addC("Failed to load weapon: ");
		DebugConsole::addC(name);
		DebugConsole::addC(" -- file: ");
		DebugConsole::addC(filename);
		DebugConsole::addToLog(" did not exist");
	}
}

void Weapon::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	if(exists == true) //If the sprite exists, change the position of that
	{
		agk::SetSpritePositionByOffset(SID, x, y);
	}
}
void Weapon::targetPos(float targetX, float targetY)
{
	float diffX = targetX - x; //Getting the distance between the target and the position
	float diffY = targetY - y;

	float nAngle = agk::ATanFull(diffY, - diffX);

	angle = nAngle;

	while(angle > 180) //Making sure angle is between 0 and 360
	{
		angle = angle - 360;
	}
	while(angle < -180)
	{
		angle = angle + 360;
	}

	//Making sure the sprite exists
	if(agk::GetSpriteExists(SID))
	{
		agk::SetSpriteAngle(SID, angle);
		if(angle < -90 || angle > 90)
		{
			agk::SetSpriteFlip(SID, 0, 1);
		}
		else
		{
			agk::SetSpriteFlip(SID, 0, 0);
		}
	}

	agk::Print(angle);
}