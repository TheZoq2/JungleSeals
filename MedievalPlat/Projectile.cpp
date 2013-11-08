#include "Projectile.h"


char projPath[128] = "Data/Projectiles/";
int proj_physType = 1;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
	int a = 0;
}

void Projectile::createFromBase(ProjectileBase* projBase, float x, float y, float angle, float speedX, float speedY, ParticleGroup* partGroup)
{
	oldX = 0;
	oldY = 0;
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

	if(projBase->getPartID() != -1)
	{
		partID = partGroup->addFromClone(projBase->getPartID());
	}

	//Impact particles
	impactPartName = new std::vector< uString >;
	impactPart = new std::vector< int >;
	std::vector< uString >* basePartNames = projBase->getImpactPartNames();

	for(unsigned int i = 0; i < basePartNames->size(); i++)
	{
		impactPartName->push_back(basePartNames->at(i));
	}

	flying = true;
}

void Projectile::setFlying(bool state)
{
	flying = false;
}

void Projectile::update()
{
	if(type == 0)
	{
		if(flying == true)
		{
			oldX = x;
			oldY = y;

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
	}
	else if(type == proj_physType)
	{
		x = agk::GetSpriteX(SID);
		y = agk::GetSpriteY(SID);
	}
}
void Projectile::updateParticle(ParticleGroup* partGroup)
{
	if(partGroup->findByID(partID) != NULL)
	{
		partGroup->setParticlePosition(partID, x, y);
	}
	
	if(impactState == 1)
	{
		for(unsigned int i = 0; i < impactPartName->size(); i++)
		{
			impactPart->push_back(partGroup->addFromFile(impactPartName->at(i), x, y));
		}

		//Stopping the creation
		impactState = 2;
	}

	if(impactState == 2)
	{
		bool removeProjectile = true;
		for(unsigned int i = 0;i < impactPart->size(); i++)
		{
			//Checking if those particles are finished
			Particle* part = partGroup->findByID(impactPart->at(i));

			if(part != NULL)
			{
				if(part->getFinished() == false)
				{
					removeProjectile = false;
				}
			}
		}

		if(removeProjectile == true)
		{
			impactState = 4;
		}
	}
}
void Projectile::updateWorld(World* world)
{
	//Checking for collision with the world
	if(flying == true && this->type != proj_physType)
	{
		for(int i = 0; i < world->getPartAmount(); i++)
		{
			//Checking many points of the trajectory
			for(float chk = 0; chk < 1; chk+=0.025f)
			{
				float xDiff = x - oldX;
				float yDiff = y - oldY;

				Part* part = world->getPartFromID(i);
				//Checking if the part is close enough
				float partDistX = x - part->getX();
				float partDistY = y - part->getY();
				float partDist = sqrt(pow(partDistX, 2) + pow(partDistY, 2));

				if(partDist < part->getEdgeRadius()) //If there is a possibility of collision with the part
				{
					if(part->getHit(oldX + xDiff*chk, oldY + yDiff*chk) && part->getPhysState() == 1)
					{
						setPosition(oldX + xDiff*chk, oldY + yDiff*chk);
						setFlying(false);

						//Hiding the sprite
						agk::SetSpriteVisible(SID, 0);

						impactState = 1;

						break;
					}
				}
			}
		}
	}
}
void Projectile::remove(ParticleGroup* partGroup) //Part group is not necessariy anymore since the projectile is not allowed to remove its own particle
{
	//Removing all particles
	if(partID != -1)
	{
		partGroup->removeParticles(partID);
	}
	for(unsigned int i = 0; i < impactPart->size(); i++)
	{
		//partGroup->removeParticles(impactPart->at(i));
	}
	
	//Removing garbage
	impactPartName->clear();
	delete impactPartName;
	impactPart->clear();
	delete impactPart;

	agk::DeleteSprite(SID);
}

void Projectile::createImpactParticles()
{

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

	if(impactState == 4)
	{
		return true;
	}
	return false;
}
void Projectile::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePositionByOffset(SID, x, y);
}

/////////////////////////////////////////////////////////////
void ProjectileBase::loadFromName(uString name, ParticleGroup* partGroup)
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
		uString partName;
		partName.SetStr("NONE");
		impactPart = new std::vector< uString >;
		partID = -1;

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
				scaleX = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("ScaleY") == 0) //ScaleY
			{
				scaleY = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Speed") == 0) //Speed
			{
				speed = float(atof(DataReader::getValue(line)));
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
				friction = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("RelativeMass") == 0)
			{
				RelativeMass = float(atof(DataReader::getValue(line)));
			}
			if(dataType.CompareTo("Trail") == 0)
			{
				partName.SetStr(DataReader::getValue(line));
			}
			if(dataType.CompareTo("ImpactPart") == 0)
			{
				int dataAmount = DataReader::getValueAmount(line);

				for(int i = 0; i < dataAmount; i++)
				{
					uString tempVal;
					tempVal.SetStr(DataReader::getValue(line, 1));

					impactPart->push_back(tempVal);
				}
			}
			delete[] line; //Removing the string from memory
		}

		DebugConsole::addToLog(filename);

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

				if(partName.CompareTo("NONE")) //If the bullet has a trail
				{
					partID = partGroup->addFromFile(partName, 20, 20);
					partGroup->setParticleVisible(partID, 0);
				}
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
int ProjectileBase::getPartID()
{
	return partID;
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
std::vector< uString >* ProjectileBase::getImpactPartNames()
{
	return this->impactPart;
}
/////////////////////////////////////////////////////////////
void ProjectileGroup::setup()
{
	projs = new std::list< Projectile >;
	projBase = new std::vector< ProjectileBase >;

	projDist = 200;

	partGroup.setup();
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

		it->updateParticle(&partGroup);

		//Checking if the projectile needs removing
		if(it->shouldBeRemoved(centerX, centerY, projDist))
		{
			removal->push_back(it);
		}
	}

	//Going thru and removing all projectiles that should be removed
	for(unsigned int i = 0; i < removal->size(); i++)
	{
		removal->at(i)->remove(&partGroup);
		projs->erase(removal->at(i));
	}

	//Updating particles
	partGroup.update();

	removal->clear();
	delete removal;
}
void ProjectileGroup::updateWorld( World* world)
{
	std::list< Projectile >::iterator it; //Looping thru the list
	for(it = projs->begin(); it != projs->end(); it++)
	{
		it->updateWorld(world);
	}
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
		tempProj.createFromBase(&projBase->at(baseIndex), x, y, angle, speedX, speedY, &partGroup);

		//Add the new projectile to the list
		projs->push_back(tempProj);
	}
	else
	{
		//Creating a new projectile base for the projectile
		ProjectileBase tempBase;
		tempBase.loadFromName(name, &partGroup);

		if(tempBase.getExists() == true)
		{
			//Add it to the vector
			projBase->push_back(tempBase);

			Projectile tempProj;
			tempProj.createFromBase(&projBase->back(), x, y, angle, speedX, speedY, &partGroup);

			//Add the new projectile to the list
			projs->push_back(tempProj);
		}
	}
}