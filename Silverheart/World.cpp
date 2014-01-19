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
	bgParts = new std::vector< Part >;

	wS = new std::vector< worldSprite >;

	entry = new std::vector< Entry >;

	clouds = new std::vector< Cloud >;

	stars = new std::vector< Star >;

	node = new std::vector< PathNode >;
	nodeLinks = new std::vector< NodeLink >;

	cloudLayers = 4;
}

void World::load(uString filename)
{
	partsToUpdate = new std::list< Part* >;
	//Starting to read the level file
	int fileID = agk::OpenToRead(filename);
		//Reading the version of the file
		char* p;//Char* to allow me to remove the string when done
		p = agk::ReadString(fileID);
		std::string version = p;
		delete[] p;

		if(version.compare("1") == 0)
		{
			p = agk::ReadString(fileID); //Special data 
			delete[] p;

			p = agk::ReadString(fileID);
			unsigned int partAmount = agk::Val(p);
			delete[] p;

			for(unsigned int i = 0; i < partAmount; i++)
			{

				agk::ReadString(fileID); //Reading the |
				std::string imgName;
				float xPos = 0;
				float yPos = 0;
				float scaleX = 1;
				float scaleY = 1;
				float angle = 0;
				int physState = 0;
				int depth = GF_BaseDepth;

				std::string name;

				//Reading the string
				std::string partData = agk::ReadString(fileID);

				std::vector< std::string > dataSegments = zString::split(partData, ";"); //Spliting the data into parts

				for(unsigned int i = 0; i < dataSegments.size(); i++)
				{
					//Splitting into type and value
					std::vector< std::string > dataParts = zString::split(dataSegments.at(i), ":");
					
					//Making sure this is actual data
					if(dataParts.size() == 2)
					{
						std::string dataType = dataParts.at(0);
						std::string dataValue = dataParts.at(1);

						if(dataType.compare("imgName") == 0)
						{
							imgName = dataValue;
						}
						else if(dataType.compare("xPos") == 0)
						{
							xPos = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("yPos") == 0)
						{
							yPos = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("scaleX") == 0)
						{
							scaleX = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("scaleY") == 0)
						{
							scaleY = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("angle") == 0)
						{
							angle = agk::ValFloat(dataValue.data());
						}
						else if(dataType.compare("physState") == 0)
						{
							physState = agk::Val(dataValue.data());
						}
						else if(dataType.compare("depth") == 0)
						{
							depth = agk::Val(dataValue.data());
						}
						else if(dataType.compare("name") == 0)
						{
							name = dataValue;
						}
						else
						{
							DebugConsole::addC("Unrecognised datatype: ");DebugConsole::addC(dataType.data());
							DebugConsole::addC(" when loading level: ");DebugConsole::addToLog(filename);
						}
					}
				}

				
				Part tempPart;

				//Checking if the sprite exists
				if(agk::GetFileExists(imgName.data()))
				{
					int cloneSprite = checkForWS(imgName.data());

					if(cloneSprite == 0)
					{
						int ID = createWS(imgName.data());

						tempPart.cloneSprite(wS->at(ID).SID);
					}
					else
					{
						tempPart.cloneSprite(wS->at(cloneSprite).SID);
					}

					tempPart.setScale(scaleX, scaleY);
					tempPart.setPosition(xPos, yPos);
					tempPart.setAngle(angle);
					tempPart.setDepth(depth);
					tempPart.setPhysState(physState);
					tempPart.setVisible(1);

					tempPart.setName(name.data());

					this->part->push_back(tempPart);
				}
			}

		}
		else
		{
			DebugConsole::addC("Failed to load level: ");DebugConsole::addC(filename);
			DebugConsole::addC(", invalid version: ");DebugConsole::addToLog(p);
		}
	
	agk::CloseFile(fileID);

	loadBG();

	//Creating pathfinding nodes
	this->generateNodes();
}

void World::update(float playerX, float playerY)
{
	updateBG(playerX, playerY);

	displayNodes();

	//Updating parts that should be updated
	//making sure the list has atleast one element
	if(partsToUpdate->size() != 0)
	{
		std::list< Part* >::iterator it; //Iterator for looping thru parts
		for(it = partsToUpdate->begin(); it != partsToUpdate->end(); it++)
		{
			Part* cPart = *it;
			cPart->update();
		}
	}
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
	partsToUpdate->clear();

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

void World::loadBG()
{
	overcast = 0.75;
	overcastTarget = overcast;

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

void World::loadMapAsBg(std::string filename)
{
	int fileID = agk::OpenToRead(filename.data());
	//Reading the version of the file
	char* p;//Char* to allow me to remove the string when done
	p = agk::ReadString(fileID);
	std::string version = p;
	delete[] p;

	if(version.compare("1") == 0)
	{
		p = agk::ReadString(fileID); //Special data 
		delete[] p;

		p = agk::ReadString(fileID);
		unsigned int partAmount = agk::Val(p);
		delete[] p;

		for(unsigned int i = 0; i < partAmount; i++)
		{

			agk::ReadString(fileID); //Reading the |
			std::string imgName;
			float xPos = 0;
			float yPos = 0;
			float scaleX = 1;
			float scaleY = 1;
			float angle = 0;
			int depth = GF_BaseDepth;

			std::string name;

			//Reading the string
			std::string partData = agk::ReadString(fileID);

			std::vector< std::string > dataSegments = zString::split(partData, ";"); //Spliting the data into parts

			for(unsigned int i = 0; i < dataSegments.size(); i++)
			{
				//Splitting into type and value
				std::vector< std::string > dataParts = zString::split(dataSegments.at(i), ":");
					
				//Making sure this is actual data
				if(dataParts.size() == 2)
				{
					std::string dataType = dataParts.at(0);
					std::string dataValue = dataParts.at(1);

					if(dataType.compare("imgName") == 0)
					{
						imgName = dataValue;
					}
					else if(dataType.compare("xPos") == 0)
					{
						xPos = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("yPos") == 0)
					{
						yPos = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("scaleX") == 0)
					{
						scaleX = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("scaleY") == 0)
					{
						scaleY = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("angle") == 0)
					{
						angle = agk::ValFloat(dataValue.data());
					}
					else if(dataType.compare("depth") == 0)
					{
						depth = agk::Val(dataValue.data());
					}
					else if(dataType.compare("name") == 0)
					{
						name = dataValue;
					}
					else
					{
						DebugConsole::addC("Unrecognised datatype: ");DebugConsole::addC(dataType.data());
						DebugConsole::addC(" when loading level as background: ");DebugConsole::addToLog(filename.data());
					}
				}
			}

				
			Part tempPart;

			//Checking if the sprite exists
			if(agk::GetFileExists(imgName.data()))
			{
				int cloneSprite = checkForWS(imgName.data());

				if(cloneSprite == 0)
				{
					int ID = createWS(imgName.data());

					tempPart.cloneSprite(wS->at(ID).SID);
				}
				else
				{
					tempPart.cloneSprite(wS->at(cloneSprite).SID);
				}

				tempPart.setScale(scaleX, scaleY);
				tempPart.setPosition(xPos, yPos);
				tempPart.setAngle(angle);
				tempPart.setDepth(200 + depth);
				tempPart.setVisible(1);

				tempPart.setName(name.data());

				bgParts->push_back(tempPart);
			}
		}

	}
	else
	{
		DebugConsole::addC("Failed to load level: ");DebugConsole::addC(filename.data());
		DebugConsole::addC(" as background, invalid version: ");DebugConsole::addToLog(p);
	}

	/*bgShader = agk::LoadShader("shaders/blur.vert", "shaders/blur.frag");
	for(unsigned int i = 0; i < bgParts->size(); i++)
	{
		agk::SetSpriteShader(bgParts->at(i).getSID(), bgShader);
	}*/
	
	agk::CloseFile(fileID);
}
void World::generateBackgroundTextures()
{
	int bgTextureAmount = 8;

	//Creating a 3d plane to generate things on
	int plane = agk::CreateObjectPlane(100, 100);
	agk::SetObjectLookAt(plane, 0, 5, 0, 0);
	agk::SetObjectColor(plane, 255, 0, 0, 255);

	agk::SetCameraPosition(1, 0, 5, 0);
	agk::SetCameraLookAt(1, 0, 0, 0, 0);

	//Moving the 2d camera away from everything
	float oldOffsetX = agk::GetViewOffsetX(); float oldOffsetY = agk::GetViewOffsetY();
	agk::SetViewOffset(-100000, -100000);

	for(int i = 0; i < 6; i++) //Generating cloud textures
	{
		
	}

	//Resetting the offset
	agk::SetViewOffset(oldOffsetX, oldOffsetY);
}
void World::updateBG(float playerX, float playerY)
{
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
	
	//Weather change
	float overcastChange = 0.00005f;

	if(abs(overcast - overcastTarget) < overcastChange)
	{
		if(rand() % 1000 == 1)
		{
			//The overcast is done changing, select a new target
			overcastTarget = agk::Random(0, 100) / 100.0f * 0.75 + 0.25;
		}
	}

	if(overcast > overcastTarget)
	{
		overcast = overcast - overcastChange;
	}
	else
	{
		overcast = overcast + overcastChange;
	}

	agk::SetShaderConstantByName(skyShader, "overcast", overcast / cloudLayers * 4, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posX", playerX / 2000 / 10, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "posY", - playerY / 2000 / 10, 0, 0, 0);
	agk::SetShaderConstantByName(skyShader, "cloudLayers", float( cloudLayers ), 5, 5, 5);

	agk::SetShaderConstantByName(skyShader, "time", time, 0, 0, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Updating the bagkround layer
	for(unsigned int i = 0; i < bgParts->size(); i++)
	{
		float xPos = bgParts->at(i).getX() + playerX / 2.0f;
		float yPos = bgParts->at(i).getY() + playerY / 2.0f - 700;
		bgParts->at(i).setDrawPos(xPos, yPos);
	}
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
void World::loadV4(char* p, int fileID)
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

			//tempNode.addLink(linkID, linkType);
		}

		//Adding the new node to the vector
		node->push_back(tempNode);
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
void World::setLastActive(int lastActive)
{
	this->lastActive = lastActive;
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
	if(partID >= 0 && partID < part->size())
	{
		return &part->at(partID);
	}
	
	return NULL;
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
int World::getLastActive()
{
	return lastActive;
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

void World::addPartToUpdate( Part* part)
{
	//Checking if the part is already in the update list
	bool alreadyInList = false;
	std::list< Part* >::iterator it;
	for(it = partsToUpdate->begin(); it != partsToUpdate->end(); it++)
	{
		if(*it == part) alreadyInList = true;
	}

	if(alreadyInList == false) //If the part wasnt in the list already
	{
		partsToUpdate->push_back(part);
	}
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

	/*for(unsigned int i = 0; i < node->size(); i++) //Going thru all the path nodes
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
	}*/

	//Going through all the node links
	for(unsigned int i = 0; i < nodeLinks->size(); i++)
	{
		//Getting the ID of the nodes
		int nodeID1 = nodeLinks->at(i).getNode(0);
		int nodeID2 = nodeLinks->at(i).getNode(1);

		PathNode* node1 = findNodeById(nodeID1);
		PathNode* node2 = findNodeById(nodeID2);

		float x1 = agk::WorldToScreenX(node1->getX());
		float y1 = agk::WorldToScreenY(node1->getY());
		float x2 = agk::WorldToScreenX(node2->getX());
		float y2 = agk::WorldToScreenY(node2->getY());

		int r = 255;
		int g = 0;
		int b = 0;

		if(nodeLinks->at(i).getType() == 1)
		{
			r = 0;
			g = 0;
			b = 255;
		}
		agk::DrawLine(x1, y1, x2, y2, r, g, b);
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

NodeLink* World::getClosestLink(float x, float y)
{
	/*
	float xOrigin = x;
	float yOrigin = y;

	float lowestDist = 100000000;

	NodeLink closestLink;
	closestLink.setNode(0, -1); //node[0] = -1;
	closestLink.setNode(1, -1); //node[1] = -1;

	unsigned int nodeAmount = this->getNodeAmount();

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
	*/
	//agk::Print(lowestDist);
	
	float lowestDist = 222222.0f;
	NodeLink* closestLink;
	for(unsigned int i = 0; i < nodeLinks->size(); i++)
	{
		PathNode* cNodes[2];
		cNodes[0] = findNodeById( nodeLinks->at(i).getNode(0) );
		cNodes[1] = findNodeById( nodeLinks->at(i).getNode(1) );

		//Making sure both nodes exist
		if(cNodes[0] != NULL && cNodes[1] != NULL)
		{
			//Calculating the k-value of the line between the nodes
			float xDiff = cNodes[1]->getX() - cNodes[0]->getX();
			float yDiff = cNodes[1]->getY() - cNodes[0]->getY();
			
			float kVal = yDiff/xDiff;
			
			for(float chk = 0; chk < xDiff; chk += 0.5f)
			{
				float xChk = cNodes[1]->getX() + chk;
				float yChk = cNodes[1]->getY() + chk*kVal;

				//Calculating the distance between the point and the location passed to the function
				float xDist = xChk - x;
				float yDist = yChk - y;

				float totDist = sqrt(pow(xDist, 2) + pow(yDist, 2));

				if(totDist < lowestDist) //If this point is closer
				{
					lowestDist = totDist;

					closestLink = &nodeLinks->at(i);
				}
			}
		}
	}

	return closestLink;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
void PathNode::create(int ID, float x, float y)
{
	this->ID = ID;
	this->x = x;
	this->y = y;

	this->linkIDs = new std::vector< int >;
}

void PathNode::addLink(int ID)
{
	linkIDs->push_back(ID);
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
	return this->linkIDs->size();
}
int PathNode::getLinkID(unsigned int slot)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void NodeLink::setNode(int index, int ID)
{
	node[index] = ID;
}
void NodeLink::setType(int type)
{
	this->type = type;
}
void NodeLink::setID(int ID)
{
	this->ID = ID;
}

int NodeLink::getNode(int index)
{
	return node[index];
}
int NodeLink::getID()
{
	return ID;
}
int NodeLink::getType()
{
	return type;
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
bool NodeLink::compareTo(NodeLink link)
{
	if( node[0] == link.getNode(0) && node[1] == link.getNode(1) )
	{
		return true;
	}
	else if(node[1] == link.getNode(0) && node[0] == link.getNode(1))
	{
		return true;
	}
	else
	{
		return false;
	}
}