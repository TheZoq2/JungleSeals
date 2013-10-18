#include "NPC.h"

#include "DebugConsole.h"

char NPC_path[128] = "Data/Characters/";

int NPC_noState = 0;
int NPC_passiveState = 1;
int NPC_walkingState = 2;

NPC::NPC(void)
{
}


NPC::~NPC(void)
{
}

void NPC::setup()
{
	formal = new std::vector< uString >;
	waypoint = new std::deque< Waypoint >;

	state = 0;
}
void NPC::update(World* world)
{
	if(exists == true) //Making sure that the NPC exists before doing anything with it
	{
		chr.update(world); //Updating the character

		this->x = chr.getX();
		this->y = chr.getY();

		if(state == NPC_noState) //The character does not have anything to do, look for something
		{
			state = NPC_passiveState;
		}
		if(state == NPC_passiveState)
		{
			float walkChanse = 100 / (frameTime*1000); //TODO: Fix this to a more reasonable value

			int randVal = rand() % 100; //Selecting a random value which should be used to determine if the NPC should walk
			if(randVal < floor(walkChanse))
			{
				//Clearing any old waypoints
				waypoint->clear();

				//Finding a good place to walk to
				float totMaxWalk = 50; //The max distance the character will walk
				
				//Selecting a direction to walk in
				int walkDir = rand() % 2;
				
				if(walkDir == 0) //Converting the 0 to a -1 because a -1 is more usefull
				{
					walkDir = -1;
				}

				//All data is gathered, figgure out how far the NPC can walk
				float currentY = chr.getFeetY();

				bool goalFound = false;

				//Finding the current X and Y cordinates
				float xPos = chr.getFeetX();
				float baseY = chr.getFeetY();
				
				agk::DrawLine(agk::WorldToScreenX(x), agk::WorldToScreenY(y), x+1, y+1, 255, 0, 0);

				bool pathFound = false;

				while(goalFound == false)
				{
					//Checking if the x / y is ground
					bool nodeFound = false; //True if a good node has been found for the current X cord

					for(float height = 0; height <= 1 && nodeFound == false; height = height + 0.1f)
					{
						for(int offsetDir = - 1; offsetDir <= 1; offsetDir += 2) //Checking both + height and - height
						{
							float offset = height * offsetDir;
							float yPos = baseY + offset;

							if(world->isGround(xPos, yPos))
							{
								Waypoint tempWaypoint; //Temporary waypoint which we will store

								tempWaypoint.setPosition(xPos, yPos); //Setting the position of the waypoint

								//Adding the waypoint to the deque
								waypoint->push_back(tempWaypoint);

								xPos = xPos + (0.5f * walkDir);
								baseY = yPos;

								pathFound = true; //We managed to find atleast one waypoint, therefore path found is true
								nodeFound = true;
							}
							else
							{
								
							}
						}
					}

					if(nodeFound == false) //No nodes were found for the current X cordinate, stop the pathfinder
					{
						goalFound = true;
					}
				}

				//Something has been found, change the state to walking
				if(pathFound == true)
				{
					state = NPC_walkingState;
				}
			}
		}
		else if(state == NPC_walkingState)
		{
			if(waypoint->size() != 0)
			{
				float lastX = waypoint->front().getX();
				float lastY = waypoint->front().getY();
				for(unsigned int i = 0; i < waypoint->size();i++) //Going thru the waypoints
				{
					//Drawing a line between the waypoints
					float startX = agk::WorldToScreenX(lastX);
					float startY = agk::WorldToScreenY(lastY);
					float endX = agk::WorldToScreenX(waypoint->at(i).getX());
					float endY = agk::WorldToScreenY(waypoint->at(i).getY());
					agk::DrawLine(startX, startY, endX, endY, 255, 0, 0);

					lastX = waypoint->at(i).getX();
					lastY = waypoint->at(i).getY();
				} //Outputting the path
			}
		}
	}
}
void NPC::updateChars(std::vector< NPC >* npc, Player* player)
{
	//Getting the distance between the NPC and the player
	float plrXDist = x - player->getX();
	float plrYDist = y - player->getY();

	float plrDist = sqrt(pow(plrXDist, 2) + pow(plrYDist, 2));
}

void NPC::createFromName(uString name)
{
	setup();
	//Getting the filename for the character
	uString filename;
	filename.Append(NPC_path);
	filename.Append(name);

	if(agk::GetFileExists(filename))
	{
		uString colSprite;

		//Starting to read the file
		int fileID = agk::OpenToRead(filename);
		
		int line = 1; //The line we are currently reading

		while(agk::FileEOF(fileID) != 1)
		{
			char* p; //The pointer which the strings from the file will be added to. Remove content before reading new line
			p = agk::ReadLine(fileID);

			//Getting the type of data on the line
			uString type;
			type.SetStr(DataReader::getType(p));
			
			if(line == 1 || type.CompareTo("comment") == 0) //If this is the first line or the line is a comment, skip it
			{

			}
			else
			{
				if(type.CompareTo("error_typeNotFound") == 0) //The line did not contain a type
				{
					DebugConsole::addC("Data error: Type not found (missing ':') at line ");
					uString $line;
					$line.Append(line);
					DebugConsole::addC($line);
					DebugConsole::addC(" In file ");
					DebugConsole::addToLog(filename);
				}
				else if(type.CompareTo("Name") == 0) //If this is a name
				{
					//Since an NPC can only have one name, the simple value function is used
					this->name.SetStr(DataReader::getValue(p));
				}
				else if(type.CompareTo("Formal") == 0)
				{
					int valueAmount = DataReader::getValueAmount(p); //Getting the amount of values

					for(int i = 0; i < valueAmount + 1; i++) //+1 since there are only commas between values and not at the end
					{
						uString value;
						value.SetStr(DataReader::getValue(p, i));

						this->formal->push_back(value);
					}
				}
				else if(type.CompareTo("Dialog") == 0)
				{

				}
				else if(type.CompareTo("ColSprite") == 0)
				{
					colSprite.SetStr(DataReader::getValue(p)); //Saving the name of the collision sprite for future use
				}
			}

			delete[] p; //Removing the string

			line++;
		}
		agk::CloseFile(fileID);

		//All the data has been loaded, using that data to create the character
		
		//Creating the colision sprite
		uString colPath;
		colPath.SetStr( GF::getPath(colSprite) ); 
		if(agk::GetFileExists(colPath))
		{
			chr.create(colPath);

			exists = true;
		}
		else
		{
			DebugConsole::addC("Could not find file: ");DebugConsole::addC(colPath);
			DebugConsole::addC(" when loading character: ");DebugConsole::addToLog(name);
		}
	}
	else
	{
		//Telling the console that the file didin't exist
		DebugConsole::addC("Failed to load character: ");DebugConsole::addC(name);DebugConsole::addC("  ---  ");DebugConsole::addToLog(filename);
	}
}
void NPC::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	chr.setPosition(x, y);
}
///////////////////////////////////////////////////////////////////////////////////////////////d////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Character::create(uString colSprite)
{
	this->colimgID = agk::LoadImage(colSprite);

	this->colSID = agk::CreateSprite(colimgID);

	//Temporary scale
	colScale = .04f;
	scaleX = colScale;
	scaleY = colScale;
	agk::SetSpriteScale(colSID, scaleX, scaleY);

	//Setting physics properties
	agk::SetSpritePhysicsOn(colSID, 2);

	agk::SetSpriteShape(colSID, 3);
	agk::SetSpritePhysicsFriction(colSID, 1.0f);
	agk::SetSpritePhysicsRestitution(colSID, 0);
	agk::SetSpritePhysicsMass(colSID, 0.1f);
	agk::SetSpriteDepth(colSID, 15);

	jumpHeight = 3;
}
void Character::update(World* world)
{
	//Preventing the collision sprite from falling over
	agk::SetSpriteAngle(colSID, 0);

	isOnGround = checkOnGround(world);

	//Updating the position of the sprite
	x = agk::GetSpriteXByOffset(colSID);
	y = agk::GetSpriteYByOffset(colSID);
}

void Character::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePosition(colSID, x, y);
}

float Character::getX()
{
	return x;
}
float Character::getY()
{
	return y;
}
float Character::getFeetY()
{
	//Getting the height of the colision sprite

	float height = agk::GetImageHeight(colimgID) * scaleY;
	float feetY = y + height/2.0f + 0.2f;
	return feetY;
}
float Character::getFeetX()
{
	return x;
}

void Character::jump()
{
	if(isOnGround && globaltime > lastJump + 0.1) //Checking if the player can jump
	{
		agk::SetSpritePhysicsImpulse(colSID, x, y, 0, -jumpHeight);

		lastJump = globaltime;
	}
}

bool Character::checkOnGround(World* world)
{
	bool canJump = false;
	//Calculating the height of the sprite
	float plrWidth = agk::GetImageWidth(colimgID) * colScale;
	float plrHeight = agk::GetImageHeight(colimgID) * colScale;

	for(int i = 0; i < world->getPartAmount(); i++)
	{
		int physState = world->getPartPhysState(i);
		if(physState != 0) //making sure that the sprite is setup for physics and part of the world
		{
			//Checking if the lower part of the sprite is coliding with anything
			int wID = world->getPartSID(i);
			for(float xChk = -0.5f; xChk < 0.5f; xChk = xChk + 0.1f)
			{
				if(agk::GetSpriteHitTest(wID, x+ xChk, y + (plrHeight / 2) + 0.5f) == 1)
				{
					canJump = true; //Looks like the character can jump
				}
			}
		}
	}

	return canJump; //Returning the result
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Waypoint::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Waypoint::getX()
{
	return x;
}
float Waypoint::getY()
{
	return y;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NPCGroup::setup()
{
	npc = new std::vector< NPC >;
}
void NPCGroup::update(World* world)
{
	for(unsigned int i = 0; i < npc->size(); i++)
	{
		npc->at(i).update(world);
	}
}
void NPCGroup::updateChars(NPCGroup* npcGroup, Player* player)
{
	//Going thru all the NPCS and updating them
	for(unsigned int i = 0; i < npc->size(); i++)
	{
		npc->at(i).updateChars(npc, player);
	}
}

void NPCGroup::addNPCFromFile(uString file)
{
	NPC tempNPC;
	tempNPC.createFromName(file);

	//Adding the NPC to the vector of NPCs
	npc->push_back(tempNPC);
}

void NPCGroup::addNPCFromFile(uString file, float x, float y)
{
	NPCGroup::addNPCFromFile(file);
	npc->back().setPosition(x, y);
}