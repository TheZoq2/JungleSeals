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
			if(DataReader::getType(line).CompareTo("Projectile") == 0) //Projectile type
			{
				projectile.SetStr(DataReader::getValue(line));
			}
			if(DataReader::getType(line).CompareTo("RateOfFire") == 0) //Rate of fire
			{
				rateOfFire = atoi(DataReader::getValue(line));
			}
			if(DataReader::getType(line).CompareTo("BarrelX") == 0) //BarrelX
			{
				uString barrelXstr;
				barrelXstr.Append(DataReader::getValue(line));

				barrelX = atof(barrelXstr);
			}
			if(DataReader::getType(line).CompareTo("BarrelY") == 0) //BarrelY
			{
				uString barrelYstr;
				barrelYstr.Append(DataReader::getValue(line));

				barrelY = atof(barrelYstr);
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

void Weapon::shoot(ProjectileGroup* projGroup)
{
	//Checking if the weapon can be shot
	float shotTime = 1 / rateOfFire;
	if(lastShot + shotTime < globaltime)
	{
		//Calculating the 0,0 position of the weapon

		//Offsetting the projectile to the barrel on the Y axis (lengnth)
		float projX = x + agk::Cos(angle - 90) * (barrelY * scaleY);
		float projY = y + agk::Sin(angle - 90) * (barrelY * scaleY);

		//Offsetting the projectile to the barrel on the X axis ("height")
		projX = projX + agk::Cos(angle) * (barrelX * scaleX);
		projY = projY + agk::Sin(angle) * (barrelX * scaleX);

		uString barrelYstr;
		barrelYstr.Append(barrelY);
		DebugConsole::addToLog(barrelYstr);

		float projAngle = angle + ((rand() % 2000) - 1000) / 1000.0 * spread;

		projGroup->addByName(projectile, projX, projY, projAngle, 0, 0);

		lastShot = globaltime;
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

	float nAngle = agk::ATanFull(diffX, diffY);

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
		if(angle < 0)
		{
			agk::SetSpriteFlip(SID, 1, 0);
		}
		else
		{
			agk::SetSpriteFlip(SID, 0, 0);
		}
	}
}