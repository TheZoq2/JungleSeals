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

	node = new std::vector< PathNode >;

	cloudLayers = 4;
}

void World::load(uString filename)
{
	//Starting to read the level file
	int fileID = agk::OpenToRead(filename);
		//Reading the version of the file
		char* p;//Char* to allow me to remove the string when done

		p = agk::ReadString(fileID); //Reading the file version
		
		if(strcmp(p, "4") == 0) //This is version 0, continue reading the file
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

			//Reading pathfinding nodes
			int nodeAmount = agk::ReadInteger(fileID);
			for(int i = 0; i < nodeAmount; i++)
			{
				PathNode tempNode;

				int ID = agk::ReadInteger(fileID);
				float xPos = agk::ReadFloat(fileID);
				float yPos = agk::ReadFloat(fileID);

				tempNode.create(ID, xPos, yPos);

				int linkAmount = agk::ReadInteger(fileID);
				for(int n = 0; n < linkAmount; n++)
				{
					int linkID = agk::ReadInteger(fileID);
					int linkType = agk::ReadInteger(fileID);

					tempNode.addLink(linkID, linkType);
				}

				//Adding the new node to the vector
				node->push_back(tempNode);
			}
		}

		loadV3(p, fileID);
		
		
	agk::CloseFile(fileID);

	loadBG();

	//Creating pathfinding nodes
	this->generateNodes();
}

void World::update(float playerX, float playerY)
{
	updateBG(playerX, playerY);

	displayNodes();
}
void World::clear()
{
	/*//Removing the background
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
	clouds->clear();*/

	//Remvoving the background
	agk::DeleteObject(skyID);

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

void World::loadV3(char* p, int fileID)
{
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
void World::setCloudLayers(int cloudLayers)
{
	this->cloudLayers = cloudLayers;
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
int World::getSkyID()
{
	return skyID;
}
float World::getTime()
{
	return time;
}
float World::getLeftEdge()
{
	float edge = 10000;
	for(unsigned int i = 0; i < part->size(); i++) //Going through all of the parts
	{
		if(part->at(i).getX() - part->at(i).getEdgeRadius() < edge) //Checking if this one is "lefter" than the others
		{
			edge = part->at(i).getX() - part->at(i).getEdgeRadius(); //Updating the edge
		}
	}
	return edge;
}
float World::getWidth()
{
	//Extreme values to make sure something is found
	float max = -100000;
	float min = 100000;

	for(unsigned int i = 0; i < part->size(); i++) //Checking all parts
	{
		float xPos = part->at(i).getX();

		//Checking if the values should be changed
		if(xPos - part->at(i).getEdgeRadius() < min)
		{
			min = xPos - part->at(i).getEdgeRadius();
		}
		if(xPos + part->at(i).getEdgeRadius() > max)
		{
			max = xPos + part->at(i).getEdgeRadius();
		}
	}

	//Calculating the width
	float width = max - min;
	return width;
}
float World::getBottomEdge()
{
	float max = -10000;
	
	for(unsigned int i = 0; i < part->size(); i++)
	{
		if(part->at(i).getY() + part->at(i).getEdgeRadius() > max)
		{
			max = part->at(i).getY() + part->at(i).getEdgeRadius();
		}
	}

	return max;
}
float World::getHeight()
{
	float max = -10000;
	float min = 10000;

	for(unsigned int i = 0; i < part->size(); i++)
	{
		float yPos = part->at(i).getY();
		
		if(yPos - part->at(i).getEdgeRadius() < min)
		{
			min = yPos - part->at(i).getEdgeRadius();
		}
		if(yPos + part->at(i).getEdgeRadius() > max)
		{
			max = yPos + part->at(i).getEdgeRadius();
		}
	}

	float height = max - min;

	return height;
}

bool World::isGround(float x, float y)
{
	bool isGround = false;
	for(unsigned int i = 0; i < part->size(); i++)
	{
		int physState = part->at(i).getPhysState();
		if(physState == 2 || physState == 1) //Checking if physics is enabled for the sprite
		{
			if(part->at(i).getHit(x, y))
			{
				isGround = true;
			}
		}
	}
	return isGround;
}

/*int World::getEntryAmount()
{
	return entry->size();
}
//World::Entry* World::getEntry(int entryID)
{
	return &entry->at(entryID);
}*/

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
	overcast = 0.75;

	/*skyR = 160;
	skyG = 200;
	skyB = 255;*/

	/*int skyImg = agk::LoadImage(GF::getPath("Background/Sky.png"));
	skyID = agk::CreateSprite(skyImg);
	agk::SetSpriteDepth(skyID, 1000); //Placing the sky at the back

	//White sky image
	lightSky = agk::CloneSprite(1);
	agk::FixSpriteToScreen(lightSky, 1);
	agk::SetSpriteScale(lightSky, agk::GetVirtualWidth(), agk::GetVirtualHeight());
	agk::SetSpriteColor(lightSky, 255, 255, 255, 255);
	agk::SetSpriteDepth(lightSky, 1000);

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
		float maxX = float( agk::GetVirtualWidth() / agk::GetViewZoom() );
		float maxY = float( agk::GetVirtualHeight() / agk::GetViewZoom() );
		tempStar.x = agk::Random(0, maxX);
		tempStar.y = agk::Random(0, maxY);
		agk::SetSpriteDepth(tempStar.SID, 999);
		agk::SetSpriteVisible(tempStar.SID, 1);

		stars->push_back(tempStar); //Adding the star to the star array
	}*/

	skyID = agk::CreateObjectPlane(100, 100);
	agk::SetObjectLookAt(skyID, 0, 5, 0, 0);
	agk::SetObjectColor(skyID, 255, 0, 0, 255);

	agk::SetCameraPosition(1, 0, 5, 0);
	agk::SetCameraLookAt(1, 0, 0, 0, 0);

	skyShader = agk::LoadShader("shaders/sky.vs", "shaders/sky.fs");
	agk::SetShaderConstantByName(skyShader, "iResolution", float( agk::GetVirtualWidth() ), float( agk::GetVirtualHeight() ), 0, 0);

	float dSkyR = 0.62f;
	float dSkyG = 0.78f;
	float dSkyB = 1.0f;

	float nSkyR = 0.05f;
	float nSkyG = 0.05f;
	float nSkyB = 0.39f;

	agk::SetShaderConstantByName(skyShader, "dSky", dSkyR, dSkyG, dSkyB, 1.0f);
	agk::SetShaderConstantByName(skyShader, "nSky", nSkyR, nSkyG, nSkyB, 1.0f);
	
	float dCloudR = 1.0f;
	float dCloudG = 1.0f;
	float dCloudB = 1.0f;
	
	float nCloudR = 0.05f;
	float nCloudG = 0.05f;
	float nCloudB = 0.05f;

	agk::SetShaderConstantByName(skyShader, "dCloud", dCloudR, dCloudG, dCloudB, 1.0f);
	agk::SetShaderConstantByName(skyShader, "nCloud", nCloudR, nCloudG, nCloudB, 1.0f);

	agk::SetObjectShader(skyID, skyShader);

	
}
void World::updateBG(float playerX, float playerY)
{
	/*agk::SetSpritePosition(skyID, agk::ScreenToWorldX(0), agk::ScreenToWorldY(0));

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

			tempCloud.x = float( agk::Random(0, cloudEndX) + cloudStartX );
			tempCloud.y = float( agk::Random(1, 30) );
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
	//Increasing time
	float timeInc = 0.5 * speedMod;
	time = time + timeInc;
	
	//Setting the color values for day/night
	int dayR = 160;
	int dayG = 200;
	int dayB = 255;

	int nightR = 25;
	int nightG = 25;
	int nightB = 100;

	//checking if it is nighttime
	if(time > 2200 || time < 600)
	{
		skyR = nightR;
		skyG = nightG;
		skyB = nightB;

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

		//Star color
		for(unsigned int i = 0; i < stars->size(); i++)
		{
			agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 255);
		}
	}
	else if(time > 600 && time < 800)
	{
		//Calculating the diffirence in color levels
		float rDiff = 255 - 5;
		float gDiff = 255 - 5;
		float bDiff = 255 - 5;

		float timeFact = (time - 600) / 200;

		float r = 5 + rDiff * timeFact;
		float g = 5 + gDiff * timeFact;
		float b = 5 + bDiff * timeFact;

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), r, g, b, 255);
		}

		//Star color
		for(unsigned int i = 0; i < stars->size(); i++)
		{
			agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 255 - 255 * timeFact);
		}

		//Sky color
		float skyRDiff = dayR - nightR;
		float skyGDiff = dayG - nightG;
		float skyBDiff = dayB - nightB;

		float skyR = nightR + skyRDiff * timeFact;
		float skyG = nightG + skyGDiff * timeFact;
		float skyB = nightB + skyBDiff * timeFact;

		agk::SetSpriteColor(skyID, skyR, skyG, skyB, 255);

		//CLoud color
		float cloudRDiff = 255 - 10;
		float cloudGDiff = 255 - 10; 
		float cloudBDiff = 255 - 10;

		float cloudR = 10 + cloudRDiff * timeFact;
		float cloudG = 10 + cloudGDiff * timeFact;
		float cloudB = 10 + cloudBDiff * timeFact;

		for(unsigned int i = 0; i < clouds->size(); i++)
		{
			agk::SetSpriteColor(clouds->at(i).SID, cloudR, cloudG, cloudB, 255);
		}
	}
	else if(time > 800 && time < 1999)
	{
		skyR = dayR;
		skyG = dayG;
		skyB = dayB;

		agk::SetSpriteColor(skyID, skyR, skyG, skyB, 255);

		for(unsigned int i = 0; i < clouds->size(); i++)
		{
			agk::SetSpriteColor(clouds->at(i).SID, 255, 255, 255, 255);
		}

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), 255, 255, 255, 255);
		}

		//Star color
		for(unsigned int i = 0; i < stars->size(); i++)
		{
			agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 0);
		}
	}
	else if( time > 2000 && time < 2200)
	{
		//Calculating the diffirence in color levels
		float rDiff = 255 - 5;
		float gDiff = 255 - 5;
		float bDiff = 255 - 5;

		float timeFact = (time - 2000) / 200;

		float r = 255 - rDiff * timeFact;
		float g = 255 - gDiff * timeFact;
		float b = 255 - bDiff * timeFact;

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), r, g, b, 255);
		}

		//Star color
		for(unsigned int i = 0; i < stars->size(); i++)
		{
			agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 255 + 255 * timeFact);
		}

		//Sky color
		float skyRDiff = dayR - nightR;
		float skyGDiff = dayG - nightG;
		float skyBDiff = dayB - nightB;

		float skyR = dayR - skyRDiff * timeFact;
		float skyG = dayG - skyGDiff * timeFact;
		float skyB = dayB - skyBDiff * timeFact;

		agk::SetSpriteColor(skyID, skyR, skyG, skyB, 255);

		//CLoud color
		float cloudRDiff = 255 - 10;
		float cloudGDiff = 255 - 10; 
		float cloudBDiff = 255 - 10;

		float cloudR = 255 - cloudRDiff * timeFact;
		float cloudG = 255 - cloudGDiff * timeFact;
		float cloudB = 255 - cloudBDiff * timeFact;

		for(unsigned int i = 0; i < clouds->size(); i++)
		{
			agk::SetSpriteColor(clouds->at(i).SID, cloudR, cloudG, cloudB, 255);
		}
	}
	////////////////////////////////////////////////////////////////////////
	//								Stars
	////////////////////////////////////////////////////////////////////////
	for(unsigned int i = 0; i < stars->size(); i++)
	{
		agk::SetSpritePosition(stars->at(i).SID, agk::ScreenToWorldX(0) + stars->at(i).x, agk::ScreenToWorldY(0) + stars->at(i).y);
	}

	////////////////////////////////////////////////////////////////////////
	//							Moon / Sun
	////////////////////////////////////////////////////////////////////////*/

////////////////////////////////////////////////////////////////////////
	//							Time
	////////////////////////////////////////////////////////////////////////
	//Making sure time is between 0 and 24
	while(time > 2400)
	{
		time = time - 2400;
	}
	//Increasing time
	float timeInc = 0.5f * speedMod;
	time = time + timeInc;
	
	//Setting the color values for day/night
	int dayR = 160;
	int dayG = 200;
	int dayB = 255;

	int nightR = 25;
	int nightG = 25;
	int nightB = 100;

	//checking if it is nighttime
	if(time > 2200 || time < 600)
	{
		/*skyR = nightR;
		skyG = nightG;
		skyB = nightB;*/

		//Part color
		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), 5, 5, 5, 255);
		}

		//Star color
		for(unsigned int i = 0; i < stars->size(); i++)
		{
			agk::SetSpriteColor(stars->at(i).SID, 255, 255, 255, 255);
		}
	}
	else if(time > 600 && time < 800)
	{
		//Calculating the diffirence in color levels
		float rDiff = 255 - 5;
		float gDiff = 255 - 5;
		float bDiff = 255 - 5;

		float timeFact = (time - 600) / 200;

		float r = 5 + rDiff * timeFact;
		float g = 5 + gDiff * timeFact;
		float b = 5 + bDiff * timeFact;

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), int( r ), int( g ), int( b ), 255);
		}
	}
	else if(time > 800 && time < 1999)
	{
		/*skyR = dayR;
		skyG = dayG;
		skyB = dayB;*/

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), 255, 255, 255, 255);
		}
	}
	else if( time > 2000 && time < 2200)
	{
		//Calculating the diffirence in color levels
		float rDiff = 255 - 5;
		float gDiff = 255 - 5;
		float bDiff = 255 - 5;

		float timeFact = (time - 2000) / 200;

		float r = 255 - rDiff * timeFact;
		float g = 255 - gDiff * timeFact;
		float b = 255 - bDiff * timeFact;

		for(unsigned int i = 0; i < part->size(); i++)
		{
			agk::SetSpriteColor(part->at(i).getSID(), int( r ), int( g ), int( b ), 255);
		}
	}
	
	agk::SetShaderConstantByName(skyShader, "overcast", overcast / cloudLayers * 4, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posX", playerX / 2000, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posY", - playerY / 2000, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "cloudLayers", float( cloudLayers ), 5, 5, 5);

	agk::SetShaderConstantByName(skyShader, "time", time, 0, 0, 0);
}

void World::setLightModeOn()
{
	//Hiding the sky
	agk::SetSpriteVisible(skyID, 0);
	agk::SetSpriteVisible(lightSky, 1);

	//Hiding the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		agk::SetSpriteVisible(clouds->at(i).SID, 0);
	}

	//Setting the color of the rest of the world
	for(unsigned int i = 0; i < part->size(); i++)
	{
		agk::SetSpriteColor(part->at(i).getSID(), 0, 0, 0, 255);
	}
}
void World::setLightModeOff()
{
	//Hiding the sky
	agk::SetSpriteVisible(skyID, 1);
	agk::SetSpriteVisible(lightSky, 0);

	//Hiding the clouds
	for(unsigned int i = 0; i < clouds->size(); i++)
	{
		agk::SetSpriteVisible(clouds->at(i).SID, 1);
	}

	//Setting the color of the rest of the world
	for(unsigned int i = 0; i < part->size(); i++)
	{
		agk::SetSpriteColor(part->at(i).getSID(), 255, 255, 255, 255);
	}
}

float World::paralaxOffset(int depth)
{
	//at depth 1000 the sprite will not move at all, at depth 100 the sprite will not have any paralax offset
	float result;

	float paralaxDepth = depth - 100.0f; //Calculating the depth if 0 is 100

	result = paralaxDepth / 900.0f;

	return result;
}

////////////////////////////////////////////////////////////////////////////
void World::generateNodes()
{
	/*
	//Clearing the old nodes
	node->clear();

	float chkScale = 0.5f;
	float leftEdge = getLeftEdge();
	float width = getLeftEdge() + getWidth();

	float bottomEdge = getBottomEdge();
	float height = getBottomEdge() - getHeight();

	for(float xCord = leftEdge; xCord < width; xCord = xCord + chkScale) //Going from the edge of the map to the oposite edge
	{
		for(float yCord = getBottomEdge(); yCord > height; yCord = yCord - chkScale)
		{
			bool nodeHere = false; //True if there is a node at this cordinate and we don't have to check further
			//Looping through all of the parts for collision checks
			for(unsigned int i = 0; i < part->size() && nodeHere == false; i++)
			{
				if(part->at(i).getPhysState() == 1) //Making sure the sprite is actually using physics
				{
					float partX = part->at(i).getX();
					float partY = part->at(i).getY();

					float partDistX = xCord - partX;
					float partDistY = yCord - partY;
					float partDist = sqrt(pow(partDistX, 2) + pow(partDistY, 2));
				
					//Checking if the part is close enough to require checking
					float radius = part->at(i).getEdgeRadius();
					if(partDist < radius)
					{
						if(part->at(i).getHit(xCord, yCord)) //Checking if the part is here
						{
							nodeHere = true; //Stopping the check in this cordinate
							
							//Creating a node
							PathNode tempNode;
							tempNode.setPos(xCord, yCord);

							//Adding the node to the node vector
							node->push_back(tempNode);
						}
					}
				}
			}
		}
	}*/
}

void World::displayNodes()
{
	/*
	for(unsigned int i = 0; i < node->size(); i++)
	{
		float xPos = agk::WorldToScreenX(node->at(i).getX());
		float yPos = agk::WorldToScreenY(node->at(i).getY());

		agk::DrawLine(xPos, yPos, xPos, yPos, 255, 0, 0);
	}
	*/

	for(unsigned int i = 0; i < node->size(); i++) //Going thru all the path nodes
	{
		float xPos = node->at(i).getX();
		float yPos = node->at(i).getY();

		unsigned int linkAmount = node->at(i).getLinkAmount(); //Getting the amount of link
		for(unsigned int n = 0; n < linkAmount; n++) //Going through those links
		{
			int linkID = node->at(i).getLinkID(n); //Getting the ID of the link

			if(linkID != -1)
			{
				//Finding that link
				PathNode* linkNode = findNodeById(linkID);

				//agk::DrawLine(agk::WorldToScreenX(xPos), agk::WorldToScreenY(yPos), agk::WorldToScreenX(linkNode->getX()), agk::WorldToScreenY(linkNode->getY()), 255, 0, 0);
			}
		}
	}
}
PathNode* World::findNodeById(int ID)
{
	for(unsigned int i = 0; i < node->size(); i++)
	{
		if(node->at(i).getID() == ID)
		{
			return &node->at(i);
		}
	}

	return NULL;
}
PathNode* World::findNodeBySlot(unsigned int slot)
{
	if(slot >= 0 && slot < node->size())
	{
		return &node->at(slot);
	}
	else
	{
		return NULL;
	}
}

unsigned int World::getNodeAmount()
{
	return node->size();
}

NodeLink World::getClosestLink(float x, float y)
{
	float xOrigin = x;
	float yOrigin = y;

	float lowestDist = 100000000;

	NodeLink closestLink;
	closestLink.setNode(0, -1); //node[0] = -1;
	closestLink.setNode(1, -1); //node[1] = -1;

	int nodeAmount = this->getNodeAmount();

	//Going through all the nodes
	for(unsigned int i = 0; i < nodeAmount; i++)
	{
		PathNode* node = this->findNodeById(i);

		float xPos = node->getX();
		float yPos = node->getY();

		//Going thru all of the links
		for(unsigned int n = 0; n < node->getLinkAmount(); n++)
		{
			//Getting the second node
			PathNode* linkNode = this->findNodeById( node->getLinkID(n) );
			
			//Calculating a function for the angle of the line
			float xDiff = linkNode->getX() - xPos;
			float yDiff = linkNode->getY() - yPos;

			float kVal = yDiff / xDiff;
			
			//Calculating lots of points of the line
			for(float xChk = 0; xChk < xDiff; xChk += 1.0f)
			{
				float yChk = xChk * kVal; //Calculating the y cordinate of 

				float xPosChk = xChk + xPos;
				float yPosChk = yChk + yPos;

				//Calculating the distance between the NPC and the point
				float NPCDistX = xPosChk - xOrigin;
				float NPCDistY = yPosChk - yOrigin;
				float NPCDist = sqrt(pow(NPCDistX, 2) + pow(NPCDistY, 2));
				
				if(NPCDist < lowestDist)
				{
					//Saving the new closest link
					lowestDist = NPCDist;

					closestLink.setNode(0, node->getID()); // node[0] = node->getID();
					closestLink.setNode(1, linkNode->getID()); //node[1] = linkNode->getID();
				}
			}
		}
	}

	if(lowestDist > 2) //If no suitable node was found
	{
		closestLink.setNode(0, -1); //node[0] = -1;
		closestLink.setNode(1, -1); //node[1] = -1;
	}

	//agk::Print(lowestDist);

	return closestLink;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void PathNode::create(int ID, float x, float y)
{
	this->ID = ID;
	this->x = x;
	this->y = y;

	this->links = new std::vector< Link >;
}

void PathNode::addLink(int ID, int type)
{
	Link tempLink;
	tempLink.ID = ID;
	tempLink.type = type;

	links->push_back(tempLink);
}

void PathNode::setPos(float x, float y)
{
	this->x = x;
	this->y = y;
}

int PathNode::getID()
{
	return ID;
}
float PathNode::getX()
{
	return x;
}
float PathNode::getY()
{
	return y;
}
unsigned int PathNode::getLinkAmount()
{
	return links->size();
}
int PathNode::getLinkID(unsigned int slot)
{
	if(slot >= 0 && slot < links->size())
	{
		return links->at(slot).ID;
	}
	else
	{
		DebugConsole::addToLog("Invalid link ID passed to 'getLinkByID'");
		return -1;
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void NodeLink::setNode(int index, int ID)
{
	node[index] = ID;
}
int NodeLink::getNode(int index)
{
	return node[index];
}

bool NodeLink::isBadLink()
{
	//CHecking if one of the nodes has not been set
	if(node[0] == -1 || node[1] == -1)
	{
		return true;
	}
	else return false;
}