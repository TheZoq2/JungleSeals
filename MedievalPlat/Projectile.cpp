#include "Projectile.h"


char projPath[128] = "Data/Projectiles/";
int proj_physType = 1;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

void Projectile::createFromBase(ProjectileBase* projBase, float x, float y, float angle, float speedX, float speedY)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->speedX = speedX;
	this->speedY = speedY;

	SID = agk::CloneSprite(projBase->getSID());

	agk::SetSpriteVisible(SID, 1);
	agk::SetSpritePositionByOffset(SID, x, y);
	agk::SetSpriteAngle(SID, angle);

	speed = projBase->getSpeed();

	type = projBase->getType();

	if(type == proj_physType)
	{
		agk::SetSpritePhysicsOn(SID, 2);

		float physSpeedX = agk::Cos(angle - 90) * speed;
		float physSpeedY = agk::Sin(angle - 90) * speed;

		agk::SetSpritePhysicsVelocity(SID, physSpeedX, physSpeedY);

		if(projBase->getFriction() != -1) agk::SetSpritePhysicsFriction(SID, projBase->getFriction());
		agk::SetSpritePhysicsMass(SID, agk::GetSpritePhysicsMass(SID) * projBase->getRelativeMass());//Setting the mass based on the generated mass
	}
}

void Projectile::update()
{
	if(type == 0)
	{
		x = x + 0;
		y = y + 0;
		//Adding the speed to the position of the projectile
		float xAdd = agk::Cos(angle - 90) * speed * speedMod;
		float yAdd = agk::Sin(angle - 90) * speed * speedMod;

		x = x + xAdd;
		y = y + yAdd;

		//Making sure the sprite exists before actually modifying it
		if(agk::GetSpriteExists(SID))
		{
			agk::SetSpritePositionByOffset(SID, x, y);
		}
	}
	else if(type == proj_physType)
	{
		x = agk::GetSpriteX(SID);
		y = agk::GetSpriteY(SID);
	}
}
void Projectile::remove()
{
	agk::DeleteSprite(SID);
}

bool Projectile::shouldBeRemoved(float centerX, float centerY, float removalDist)
{
	float distX = this->x - centerX;
	float distY = this->y - centerY;

	float dist = sqrt(pow(distX, 2) + pow(distY, 2));

	if(dist > removalDist)
	{
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////
void ProjectileBase::loadFromName(uString name)
{
	uString filename;
	filename.SetStr(projPath);
	filename.Append(name);

	//Making sure the projectile file exists
	if(agk::GetFileExists(filename))
	{
		int colorR = -1;
		int colorG = -1;
		int colorB = -1;


		uString spriteName;
		spriteName.SetStr(" ");
		type = 0;
		friction = -1;
		RelativeMass = 1;

		//Starting to read the file
		int fileID = agk::OpenToRead(filename);
		
		while(agk::FileEOF(fileID) == 0)
		{
			char* line;

			//Reading the line
			line = agk::ReadLine(fileID);

			//Checking the line with the datareader
			uString dataType;
			dataType.SetStr(DataReader::getType(line));

			if(dataType.CompareTo("Sprite") == 0) //Spritename
			{
				spriteName.SetStr(DataReader::getValue(line));
			}
			if(dataType.CompareTo("Type") == 0)
			{
				type = proj_physType;
			}
			if(dataType.CompareTo("ScaleX") == 0) //ScaleX
			{
				scaleX = atof(DataReader::getValue(line));
			}
			if(dataType.CompareTo("ScaleY") == 0) //ScaleY
			{
				scaleY = atof(DataReader::getValue(line));
			}
			if(dataType.CompareTo("Speed") == 0) //Speed
			{
				speed = atof(DataReader::getValue(line));
			}
			if(dataType.CompareTo("ColorR") == 0)//Colors
			{
				colorR = atoi(DataReader::getValue(line));
			}
			if(dataType.CompareTo("ColorG") == 0)//Colors
			{
				colorG = atoi(DataReader::getValue(line));
			}
			if(dataType.CompareTo("ColorB") == 0)//Colors
			{
				colorB = atoi(DataReader::getValue(line));
			}
			if(dataType.CompareTo("Friction") == 0)
			{
				friction = atof(DataReader::getValue(line));
			}
			if(dataType.CompareTo("RelativeMass") == 0)
			{
				RelativeMass = atof(DataReader::getValue(line));
			}

			delete[] line; //Removing the string from memory
		}

		agk::CloseFile(fileID);

		//The file has been read, creating the actual projectile base
		
		//Making sure a sprite was set
		if(spriteName.CompareTo(" ") != 0)
		{
			this->name.SetStr(name);

			//Making sure the image exists
			uString imgFile;
			imgFile.SetStr(GF::getPath(spriteName));

			if(agk::GetFileExists(imgFile))
			{
				//The image did exist
				imgID = agk::LoadImage(imgFile);
				SID = agk::CreateSprite(imgID);

				agk::SetSpriteScale(SID, scaleX, scaleY);
				agk::SetSpriteDepth(SID, 15);

				exists = true; //Saying that the projectile base exists and can be used

				if(colorR != -1 || colorG != -1 || colorB != -1)
				{
					agk::SetSpriteColor(SID, colorR, colorG, colorB, 255);
				}

				agk::SetSpriteVisible(SID, 0);
			}
			else
			{
				//The sprite didn't exist, alert the user
				DebugConsole::addC("The image: '");
				DebugConsole::addC(imgFile);
				DebugConsole::addC("' for projectile: '");
				DebugConsole::addC(name);
				DebugConsole::addC("' did not exist -- ");
				DebugConsole::addToLog(filename);
			}
		}
		else
		{
			DebugConsole::addC("No sprite was set for projectile: ");
			DebugConsole::addC(name);
			DebugConsole::addC(" -- ");
			DebugConsole::addToLog(filename);
		}
	}
	else
	{
		//Notifiying the user that the projectile does not exist
		DebugConsole::addC("Projectile: ");
		DebugConsole::addC(name);
		DebugConsole::addC(" does not exist -- ");
		DebugConsole::addToLog(filename);
	}
}

uString ProjectileBase::getName()
{
	return name;
}
bool ProjectileBase::getExists()
{
	return exists;
}
int ProjectileBase::getSID()
{
	return SID;
}
float ProjectileBase::getSpeed()
{
	return speed;
}
int ProjectileBase::getType()
{
	return this->type;
}
float ProjectileBase::getFriction()
{
	return friction;
}
float ProjectileBase::getRelativeMass()
{
	return RelativeMass;
}
/////////////////////////////////////////////////////////////
void ProjectileGroup::setup()
{
	projs = new std::list< Projectile >;
	projBase = new std::vector< ProjectileBase >;

	projDist = 2000;
}

void ProjectileGroup::update( float centerX, float centerY )
{
	std::vector< std::list< Projectile >::iterator >* removal; //Contains all the projectiles that should be removed. This will be done after all projectiles
	removal = new std::vector< std::list< Projectile >::iterator >;
	//Have been updated to avoid any weird issues

	std::list< Projectile >::iterator it; //Looping thru the list
	for(it = projs->begin(); it != projs->end(); it++)
	{
		it->update();

		//Checking if the projectile needs removing
		if(it->shouldBeRemoved(centerX, centerY, projDist))
		{
			removal->push_back(it);
		}
	}

	//Going thru and removing all projectiles that should be removed
	for(unsigned int i = 0; i < removal->size(); i++)
	{
		removal->at(i)->remove();
		projs->erase(removal->at(i));
	}
	
	removal->clear();
	delete removal;
}

void ProjectileGroup::addByName(uString name, float x, float y, float angle, float speedX, float speedY)
{
	//Checking if this kind of projectile exists already
	bool baseExists = false;
	unsigned int baseIndex;

	for(unsigned int i = 0; i < projBase->size(); i++)
	{
		if(name.CompareTo(projBase->at(i).getName()) == 0) //If the name of the projectile base is the same as the one we want to create
		{
			baseExists = true;
			baseIndex = i;
		}
	}

	if(baseExists) //If the base existed already
	{
		//Create a new projectile from the base
		Projectile tempProj;
		tempProj.createFromBase(&projBase->at(baseIndex), x, y, angle, speedX, speedY);

		//Add the new projectile to the list
		projs->push_back(tempProj);
	}
	else
	{
		//Creating a new projectile base for the projectile
		ProjectileBase tempBase;
		tempBase.loadFromName(name);

		if(tempBase.getExists() == true)
		{
			//Add it to the vector
			projBase->push_back(tempBase);

			Projectile tempProj;
			tempProj.createFromBase(&projBase->back(), x, y, angle, speedX, speedY);

			//Add the new projectile to the list
			projs->push_back(tempProj);
		}
	}
}