#include "NPC.h"

#include "DebugConsole.h"

char NPC_path[128] = "Data/Characters/";

int NPC_noState = 0;
int NPC_passiveState = 1;
int NPC_walkingState = 2;
int NPC_hasGoalState = 3;

NPC::NPC(void)
{
}


NPC::~NPC(void)
{
}

void NPC::setup()
{
	formal = new std::vector< uString >;
	path = new std::deque< int >;

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
		else if(state == NPC_passiveState)
		{
			/*float walkChanse = 100 / (frameTime*1000); //TODO: Fix this to a more reasonable value

			int randVal = rand() % 100; //Selecting a random value which should be used to determine if the NPC should walk
			randVal = 0;
			if(randVal < floor(walkChanse))
			{
				
				//Finding a suitable path to the 
				NodeLink closestPath = findClosestNodes(world);

				//Making sure there was a node relativley nearby
				if(closestPath.node[0] != -1)
				{
					PathNode* node[2];
					node[0] = world->findNodeById(closestPath.node[0]);
					node[1] = world->findNodeById(closestPath.node[1]);
					//Drawing the node link
					
					//agk::DrawLine(agk::WorldToScreenX(node[0]->getX()), agk::WorldToScreenY(node[0]->getY()), agk::WorldToScreenX(node[1]->getX()), agk::WorldToScreenY(node[1]->getY()), 255, 0, 0);

					//Selecting a random node to walk to
					int targetNode = agk::Random(0, 1);

					path->push_back(closestPath.node[targetNode]);

					state = NPC_walkingState;
				}
				
			}*/

			if(hasGoal == true)//Checking if the NPC has a goal
			{
				if(findPathToGoal(world) == true)
				{
					state = NPC_walkingState;
				}
			}

			
			//Clearing the path
			path->clear();

			//Finding somewhere to walk
			//Gettingt the nodes that the NPC is currently standing on
			NodeLink* cLink = world->getClosestLink(chr.getFeetX(), chr.getFeetY());

			if(cLink->getNode(0) != -1 && cLink->getNode(1) != -1) //Making sure that the AI has a node nearby before doing any pathfinding
			{
				/*
				//Selecting a direction to walk in
				int walkDir = rand() % 2;

				int targetNode = cLink->getNode(walkDir);

				path->push_back(targetNode);

				hasGoal = true;
				state = NPC_walkingState;

				//Calculating the goal
				goalX = world->findNodeById(targetNode)->getX();
				goalY = world->findNodeById(targetNode)->getY();
				*/
			}
		}
		else if(state == NPC_walkingState)
		{
			/*if(path->size() != 0)
			{
				
				//Getting the target node
				PathNode* targetNode = world->findNodeById(path->at(0));

				float targetX = targetNode->getX();

				float targetDiff = targetX - this->x;

				if(targetDiff < 0)
				{
					chr.walkLeft();
				}
				else if(targetDiff > 0)
				{
					chr.walkRight();
				}
				
				//Checking if the node has been reached

				float xDist = chr.getFeetX() - targetNode->getX();
				float yDist = chr.getFeetY() - targetNode->getY();
				float totDist = sqrt(pow(xDist, 2) + pow(yDist, 2));

				if(totDist < 2)
				{
					//Remove this node
					path->pop_front();
				}

				//Highlight the path
				for(unsigned int i = 0; i < path->size(); i++)
				{
					PathNode* cNode = world->findNodeById(path->at(i));

					float xPos = cNode->getX();
					float yPos = cNode->getY();

					for(unsigned int n = 0; n < cNode->getLinkAmount(); n++)
					{
						PathNode* lNode = world->findNodeById(cNode->getLinkID(n));

						float xPos2 = lNode->getX();
						float yPos2 = lNode->getY();

						agk::DrawLine(agk::WorldToScreenX(xPos), agk::WorldToScreenY(yPos), agk::WorldToScreenX(xPos2), agk::WorldToScreenY(yPos2), 255, 0, 0);
					}
				}
				
			}
			else
			{
				/*
				//The last node has been reached, move the final distance to the goal node
				if(chr.getFeetX() > goalX)
				{
					chr.walkLeft();
				}
				else
				{
					chr.walkRight();
				}

				//Checking if the goal has been reached
				float xDist = goalX - chr.getFeetX();
				float yDist = goalY - chr.getFeetY();
				float totDist = sqrt(pow(xDist, 2) + pow(yDist, 2));

				if(totDist < 3.5)
				{
					//Exiting walk state
					state = NPC_passiveState;
				}
				
			}
			*/
		}
	}
}
void NPC::updateChars(std::vector< NPC >* npc, Player* player)
{
	//Getting the distance between the NPC and the player
	float plrXDist = x - player->getX();
	float plrYDist = y - player->getY();

	float plrDist = sqrt(pow(plrXDist, 2) + pow(plrYDist, 2));

	if(state == NPC_passiveState) //The NPC has nothing to do, lets follow the player
	{
		setGoal(player->getX(), player->getY() + 2);

		hasGoal = true;
	}
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

void NPC::setGoal(float goalX, float goalY)
{
	this->goalX = goalX;
	this->goalY = goalY;
}
bool NPC::findPathToGoal(World* world)
{
	//Finding the liks that are closest to the start and goal
	NodeLink* goalLink = world->getClosestLink(goalX, goalY);
	NodeLink* startLink = world->getClosestLink(chr.getFeetX(), chr.getFeetY());

	//Making sure none of the links are good
	if(goalLink->isBadLink() || startLink->isBadLink())
	{
		return false; //Exit the function
	}

	//If the goal is the same lik as the start
	if(goalLink->compareTo(*startLink))
	{
		return true; //Return a sucessfull path
	}

	//Both the goal and start are reachable
	float lowestDist = 10000000;
	PathNode* startNode; //The link tos art the search from

	//Finding a good starting node
	for(int i = 0; i < 2; i++)
	{

	}

	char* p = agk::Str(goalLink->getID());
	DebugConsole::addC("Links have been found: ");DebugConsole::addC(p);
	delete[] p;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	//Preventing collisioin between this and other characters
	agk::SetSpriteCategoryBit(colSID, 1, 0);
	agk::SetSpriteCategoryBit(colSID, GF_charGroup, 1);
	agk::SetSpriteCollideBit(colSID, GF_charGroup, 0);

	jumpHeight = 3;
	cSpeed = 1.0f;
}
void Character::update(World* world)
{
	//Preventing the collision sprite from falling over
	agk::SetSpriteAngle(colSID, 0);

	isOnGround = checkOnGround(world);

	//Updating the position of the sprite
	x = agk::GetSpriteXByOffset(colSID);
	y = agk::GetSpriteYByOffset(colSID);

	//Making sure that the left/right speed is not to big
	float chkSpeed = 15.0f * cSpeed;
	if(agk::GetSpritePhysicsVelocityX(colSID) > chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(colSID, chkSpeed, agk::GetSpritePhysicsVelocityY(colSID));
	}
	if(agk::GetSpritePhysicsVelocityX(colSID) < -chkSpeed)
	{
		agk::SetSpritePhysicsVelocity(colSID, -chkSpeed, agk::GetSpritePhysicsVelocityY(colSID));
	}
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
void Character::walkLeft()
{
	float moveForce = 30.0f;
	//agk::SetSpritePhysicsImpulse(SID, x, y, -0.5f, 0);
	agk::SetSpritePhysicsForce(colSID, x, y, -moveForce, 0);

	cSpeed = 0.5f;
}
void Character::walkRight()
{
	float moveForce = 30.0f;
	agk::SetSpritePhysicsForce(colSID, x, y, moveForce, 0);

	cSpeed = 0.5f;
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