#include "World.h"


World::World(void)
{
}


World::~World(void)
{
}

void World::begin()
{
	part = new std::vector< Part >;

	wS = new std::vector< worldSprite >;

	entry = new std::vector< Entry >;

	clouds = new std::vector< Cloud >;

	stars = new std::vector< Star >;
}

void World::load(uString filename)
{
	//Starting to read the level file
	int fileID = agk::OpenToRead(filename);
		//Reading the version of the file
		char* p;//Char* to allow me to remove the string when done

		p = agk::ReadString(fileID); //Reading the file version
		
		if(strcmp(p, "3") == 0) //This is version 0, continue reading the file
		{
			delete[] p; //Removing the data pointed to by p

			p = agk::ReadString(fileID); //Reading the name of the level
			name.SetStr(p);
			delete[] p;
			p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused
			p = agk::ReadString(fileID); delete[] p; //Backgrounds -- Unused

			//Reading the fun stuff
			p = agk::ReadString(fileID); //Reading the amount of parts in the world
			int amount = atoi(p);
			delete[] p;

			for(int i = 0; i < amount; i++)
			{
				//Creating a temporary variable which will be pushed back into the vector once everything is set up
				Part tempPart;
				//Reading the filename
				//uString filename = agk::ReadString(fileID);

				uString filename;
				filename.SetStr(agk::ReadString(fileID));

				//Checking if the filename already exists
				//Making sure the sprite we are trying to load actually exists

				bool fileExist = true;
				//Making sure that the file exists
				if(agk::GetFileExists(filename) == 0)
				{
					fileExist = false;

					//If it dosn't exist, write that to a debug file
					int debugID = agk::OpenToWrite("debug.txt", 1);
					agk::WriteString(debugID, "\n The sprite ");
					agk::WriteString(debugID, filename.GetStr());
					agk::WriteString(debugID, " didn't exist");
					agk::CloseFile(debugID);
				}

				if(fileExist == true) //Making sure that the file we are loading exists
				{
					int cloneSprite = checkForWS(filename);
					if(cloneSprite == 0) //Checking if the filename exists
					{
						//it didn't exist, we have to create it
						int ID = createWS(filename);

						tempPart.cloneSprite(wS->at(ID).SID);
					}
					else
					{
						//It existed, we can just clone the old sprite
						tempPart.cloneSprite(wS->at(cloneSprite).SID);
					}
				}

				//A sprite has been created, let's set some parameters for it
				p = agk::ReadString(fileID); float x = agk::ValFloat(p); //Reading the x position
				delete[] p;
				p = agk::ReadString(fileID); float y = agk::ValFloat(p);
				delete[] p;

				p = agk::ReadString(fileID); int depth = agk::Val(p); //Reading the depth
				delete[] p;

				p = agk::ReadString(fileID); float scaleX = agk::ValFloat(p); //Reading the scale
				delete[] p;
				p = agk::ReadString(fileID); float scaleY = agk::ValFloat(p);
				delete[] p;

				p = agk::ReadString(fileID); float angle = agk::ValFloat(p); //Reading the angle
				delete[] p;

				p = agk::ReadString(fileID); int physState = agk::Val(p); //Reading the physics state
				delete[] p;


				////////////////////////////////////////////////////////////////////////////////////
				char* name = agk::ReadString(fileID);//Reading the name

				p = agk::ReadString(fileID); int usable = agk::Val(p); //Reading the usability
				delete[] p;

				char* actScript = agk::ReadString(fileID); //Reading the activation script

				char* useMsg =  agk::ReadString(fileID); //Reading the use message
				
				char* labels[5];
				//Reading the labels
				for(int lbl = 0; lbl < 5; lbl++)
				{
					labels[lbl] = agk::ReadString(fileID);
				}

				if(fileExist == true) //Since the file existed, we will set data to the new sprite
				{
					tempPart.setScale(scaleX, scaleY);
					tempPart.setPosition(x, y); //Setting the position
					tempPart.setDepth(depth); //Setting the depth
					tempPart.setAngle(angle);
					tempPart.setPhysState(physState);
					tempPart.setVisible(1);

					//Setting script paramenters
					tempPart.setName( name ); 
					tempPart.setUsable( usable );
					tempPart.setActScript(actScript);
					tempPart.setUseMsg(useMsg);

					for(int lbl = 0; lbl < 5; lbl++)
					{
						tempPart.setLabel(lbl, labels[lbl]);
					}

					part->push_back(tempPart);
				}

				//Cleaning up that garbage
				delete[] name;
				delete[] actScript;
				delete[] useMsg;
				for(int lbl = 0; lbl < 5; lbl++)
				{
					delete[] labels[lbl];
				}
			}

			/*
			//Reading the spawnpoints
			int entryAmount = agk::ReadInteger(fileID);
			for(int i = 0; i < entryAmount; i++)
			{
				uString name = agk::ReadString(fileID);
				float x = agk::ReadFloat(fileID);
				float y = agk::ReadFloat(fileID);

				Entry tempEntry;
				tempEntry.name.SetStr(name);
				tempEntry.x = x;
				tempEntry.y = y;

				entry->push_back(tempEntry);
			}
			*/
		}
		
		
	agk::CloseFile(fileID);

	loadBG();
}
void World::update(float playerX, float playerY)
{
	updateBG(playerX, playerY);
}
void World::clear()
{
	//Removing the background
	if(agk::GetSpriteExists(skyID))
	{
		agk::DeleteSprite(skyID);
	}

	//Removing the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		if(agk::GetSpriteExists(clouds->at(i).SID))
		{
			agk::DeleteSprite(clouds->at(i).SID);
		}
	}
	//Clearing the cloud vector
	clouds->clear();

	for(unsigned int i = 0; i < part->size(); i++)
	{
		part->at(i).remove();
	}

	part->clear();

	//Removing stars
	for(unsigned int i = 0; i < stars->size(); i++)
	{
		if(agk::GetSpriteExists(stars->at(i).SID))
		{
			agk::DeleteSprite(stars->at(i).SID);
		}
	}

	stars->clear();
}

int World::checkForWS(uString filename)
{
	int ID = 0;

	for(unsigned int i = 0; i < wS->size(); i++)
	{
		if(strcmp(wS->at(i).file.GetStr(), filename.GetStr()) == 0)
		{
			ID = i;
		}
	}

	return ID;
}

int World::createWS(uString filename)
{
	int ID;

	ID = wS->size(); //getting the index of the new worldSprite

	worldSprite tempWS;
	tempWS.file.SetStr(filename.GetStr());
	tempWS.imgID = agk::LoadImage(filename);
	tempWS.SID = agk::CreateSprite(tempWS.imgID);

	//Hiding the sprite for now
	agk::SetSpriteVisible(tempWS.SID, 0);

	//everything has been loaded, add it to the vector and return the ID
	wS->push_back(tempWS);
	return ID;
}

World::Entry* World::findEntry(uString name)
{
	int slot = 0;

	for(unsigned int i = 0; i < entry->size(); i++)
	{
		if(name.CompareTo(entry->at(i).name) == 0)
		{
			slot = i;
		}
	}

	return &entry->at(slot);
}

void World::setOvercast(float overcast)
{
	this->overcast = overcast;
}
void World::setTime(float time)
{
	this->time = time;
}

Part* World::getPartFromName(uString name) //This function goes thru all the parts and looks for one with the name specified //It will return the first part with the name
{
	Part* fPart = 0; //Will be 0 or null if no part is found

	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getName().CompareTo(name) == 0) //Checking if the names are the same
		{
			fPart = &part->at(i);
			return fPart;
		}
	}

	return fPart;
}
Part* World::getPartFromID(int partID)
{
	return &part->at(partID);
}

int World::getPartAmount()
{
	return part->size();
}
int World::getPartSID(int partID)
{
	return part->at(partID).getSID();
}
int World::getPartPhysState(int partID)
{
	return part->at(partID).getPhysState();
}
int World::getPartUsable(int partID)
{
	return part->at(partID).getUsable();
}
float World::getPartX(int partID)
{
	return part->at(partID).getX();
}
float World::getPartY(int partID)
{
	return part->at(partID).getY();
}

int World::getEntryAmount()
{
	return entry->size();
}
World::Entry* World::getEntry(int entryID)
{
	return &entry->at(entryID);
}

uString World::getName()
{
	return name;
}

void World::loadBaseMedia() //Loads media that is used in all worlds
{
	//Creating a vector of the cloud sprites
	cloudBase = new std::vector< CloudBase >;

	//Loading the media
	CloudBase tempCB;
	tempCB.img = agk::LoadImage(GF::getPath("Background/cloud1.png"));
	tempCB.SID = agk::CreateSprite(tempCB.img);
	agk::SetSpriteDepth(tempCB.SID, 900);
	agk::SetSpriteScale(tempCB.SID, 0.07f, 0.07f);
	agk::SetSpriteVisible(tempCB.SID, 0);

	//Adding the new cloud to the vector
	cloudBase->push_back(tempCB);

	//Creating the next cloud
	tempCB.img = agk::LoadImage(GF::getPath("Background/cloud2.png"));
	tempCB.SID = agk::CreateSprite(tempCB.img);
	agk::SetSpriteDepth(tempCB.SID, 900);
	agk::SetSpriteScale(tempCB.SID, 0.07f, 0.07f);
	agk::SetSpriteVisible(tempCB.SID, 0);

	//Adding the new cloud to the vector
	cloudBase->push_back(tempCB);

}
void World::loadBG()
{
	overcast = 0.25;

	skyR = 160;
	skyG = 200;
	skyB = 255;

	int skyImg = agk::LoadImage(GF::getPath("Background/Sky.png"));
	skyID = agk::CreateSprite(skyImg);
	agk::SetSpriteDepth(skyID, 1000); //Placing the sky at the back

	//Filling the screen with the sprite
	agk::SetSpriteScale(skyID, (float)agk::GetDeviceWidth(), 1.0f);

	agk::SetSpriteColor(skyID, skyR, skyG, skyB, 255);

	//Calculating the size of the world
	
	minX = 0.0; //The part cordinate with the lowest x
	maxX = 0.0; //The part cordinate with the highest x
	//Calculating the width of the world
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getX() < minX) //If this is the lowest part found
		{
			minX = part->at(i).getX(); //Update the lowest variable
		}

		if(part->at(i).getX() > maxX) //If this is the highest part found
		{
			maxX = part->at(i).getX(); //Update the lowest variable
		}
	}

	xDist = maxX - minX;

	//Creating stars
	int starAmount = 500;

	for(int i = 0; i < starAmount; i++)
	{
		//Creating a temporary star
		Star tempStar;
		tempStar.SID = agk::CloneSprite(1);
		agk::SetSpriteScale(tempStar.SID, 1 / agk::GetViewZoom(), 1 / agk::GetViewZoom());
		float maxX = agk::GetVirtualWidth() / agk::GetViewZoom();
		float maxY = agk::GetVirtualHeight() / agk::GetViewZoom();
		tempStar.x = agk::Random(0, maxX);
		tempStar.y = agk::Random(0, maxY);
		agk::SetSpriteDepth(tempStar.SID, 999);
		agk::SetSpriteVisible(tempStar.SID, 1);

		stars->push_back(tempStar); //Adding the star to the star array
	}
}
void World::updateBG(float playerX, float playerY)
{
	agk::SetSpritePosition(skyID, agk::ScreenToWorldX(0), agk::ScreenToWorldY(0));

	//Drawing the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		float xPos = clouds->at(i).x + paralaxOffset(clouds->at(i).depth) * playerX;
		float yPos = clouds->at(i).y + paralaxOffset(clouds->at(i).depth) * playerY;
		agk::SetSpritePosition(clouds->at(i).SID, xPos, yPos);
	}

	float cloudsPerUnit = 0.5f;

	//Calculating the amount of clouds that should be in the world
	float cloudAmount = cloudsPerUnit * xDist * overcast;

	float cloudStartX = minX - 50;
	float cloudEndX = maxX + 50;

	//Checking if more clouds should be created
	if(clouds->size() < cloudAmount)
	{
		//Creating more clouds
		for(int i = clouds->size(); i < cloudAmount; i++)
		{
			//Selecting a random cloud type
			int cloudType = agk::Random(0, cloudBase->size() - 1);

			//Temporary cloud
			Cloud tempCloud;
			tempCloud.SID = agk::CloneSprite(cloudBase->at(cloudType).SID);

			tempCloud.x = (float) agk::Random(0, cloudEndX) + cloudStartX;
			tempCloud.y = (float) agk::Random(1, 30);
			tempCloud.y = tempCloud.y - 40;
		
			//Setting the depth of the cloud
			tempCloud.depth = agk::Random(760, 840);
			agk::SetSpriteDepth(tempCloud.SID, tempCloud.depth);

			agk::SetSpriteVisible(tempCloud.SID, 1);
			//Adding it to the vector
			clouds->push_back(tempCloud);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//							Time
	////////////////////////////////////////////////////////////////////////
	//Making sure time is between 0 and 24
	while(time > 2400)
	{
		time = time - 2400;
	}
	
	//checking if it is nighttime
	if(time > 2000 || time < 600)
	{
		skyR = 25;
		skyG = 25;
		skyB = 100;

		agk::SetSpriteColor(skyID, skyR, skyG, skyB, 255);

		//making the clouds really dark
		for(unsigned int i = 0; i < clouds->size(); i++)
		{
			agk::SetSpriteColor(clouds->at(i).SID, 10, 10, 10, 255);
		}

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), 5, 5, 5, 255);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//								Stars
	////////////////////////////////////////////////////////////////////////
	for(unsigned int i = 0; i < stars->size(); i++)
	{
		agk::SetSpritePosition(stars->at(i).SID, agk::ScreenToWorldX(0) + stars->at(i).x, agk::ScreenToWorldY(0) + stars->at(i).y);
	}
}

float World::paralaxOffset(int depth)
{
	//at depth 1000 the sprite will not move at all, at depth 100 the sprite will not have any paralax offset
	float result;

	float paralaxDepth = depth - 100.0; //Calculating the depth if 0 is 100

	result = paralaxDepth / 900.0f;

	return result;
}