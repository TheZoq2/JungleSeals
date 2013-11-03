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
		if(state == NPC_passiveState)
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
		}
		else if(state == NPC_walkingState)
		{
			if(path->size() != 0)
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
	//Finding the closest nodes to the goal
	NodeLink goalLink;
	
	goalLink = world->getClosestLink(goalX, goalY);

	//Getting the current link
	NodeLink cLink;
	cLink = world->getClosestLink(chr.getFeetX(), chr.getFeetY());

	//If one of the positions are to far away
	if(cLink.isBadLink() || goalLink.isBadLink())
	{
		return false; //Exiting the function
	}

	//Checking if the links are the same
	if(goalLink.compareTo(cLink) == true)
	{
		return true;
	}

	//No nodes were bad, start to look for a path between the nodes
	float lowestDist = 1000000000;

	PathNode* goalNode;
	PathNode* startNode;

	//Finding a good starting node
	for(int i = 0; i < 2; i++)
	{
		PathNode* cGoalNode = world->findNodeById(goalLink.getNode(i));
		for(int n = 0; n < 2; n++)
		{
			PathNode* cStartNode = world->findNodeById(cLink.getNode(n));

			float xDist = cGoalNode->getX() - cStartNode->getX();
			float yDist = cGoalNode->getY() - cStartNode->getY();

			float dist = sqrt(pow(xDist, 2) + pow(yDist, 2));

			//If these nodes are closer than the old ones
			if(dist < lowestDist)
			{
				//Saving the new nodes
				lowestDist = dist;
				goalNode = cGoalNode;
				startNode = cStartNode;
			}
		}
	}

	//Making sure nothing weird happned
	if(goalNode == NULL || startNode == NULL)
	{
		return false;
	}

	//Everything is good, lets start looking for a good node
	int openState = 1;
	int closedState = 2;
	

	std::vector< listElement >* nodeList;
	nodeList = new std::vector< listElement >;

	//Adding the starting node to the open list
	listElement tempElement;
	tempElement.node = startNode->getID();
	tempElement.state = openState;
	tempElement.FScore = 0; //Total score
	tempElement.HScore = 0; //Estemated distance left
	tempElement.GScore = 0; //Walk cost
	tempElement.parent = -1;

	nodeList->push_back(tempElement); //Add the first node to the list

	bool pathFound = false;
	int finalNode = -1; //The final node in the chain

	while(pathFound == false) //Running the actual pathfinder
	{
		float lowestFScore = 100000000;

		int nextNode = -1;
		int listSlot = -1;

		//Finding the node with the lowest FScore
		for(unsigned int i = 0; i < nodeList->size(); i++)
		{
			if(nodeList->at(i).state == openState && nodeList->at(i).FScore < lowestFScore)
			{
				lowestFScore = nodeList->at(i).FScore;

				nextNode = nodeList->at(i).node;
				listSlot = i;
			}
		}

		if(nextNode == -1) //No open nodes left, a path could not be found
		{
			//Removing garbage
			nodeList->clear();
			delete nodeList;
			return false;
		}

		PathNode* cNode = world->findNodeById(nextNode);
		//PathNode* cNode = goalNode;

		//Open the nodes linked to the current node
		for(unsigned int i = 0; i < cNode->getLinkAmount(); i++)
		{
			int node = cNode->getLinkID(i);

			//Checking if the node is already on the list
			bool onList = false;
			int listIndex;
			int listState = 0;

			for(unsigned int n = 0; n < nodeList->size(); n++)
			{
				if(nodeList->at(n).node == node) //If the node is on the list
				{
					onList = true;

					listIndex = n;
					listState = nodeList->at(n).state;
				}
			}
			
			//This is a new node or it is open
			//if(onList == false)
			//{
				//Calculating the walk cost between this and the next node
				PathNode* thisNode = world->findNodeById(node);

				float xDist = thisNode->getX() - cNode->getX();
				float yDist = thisNode->getY() - cNode->getY();
				float hScore = sqrt(pow(xDist, 2) + pow(yDist, 2)) + nodeList->at(listSlot).HScore;
				
				//If the node was on the list already
				if(onList == true && listState == openState)
				{
					//Checking if this has a better HScore
					if(hScore < nodeList->at(listIndex).HScore)
					{
						//Making this node the parent instead
						nodeList->at(listIndex).HScore = hScore;
						nodeList->at(listIndex).FScore = hScore + nodeList->at(listSlot).GScore;

						nodeList->at(listIndex).parent = listSlot;
					}
				}
				else if(listState == closedState) //Ignoring closed nodes
				{}
				else
				{
					thisNode = world->findNodeById(node);;
					//Calculating the score
					float xDist = thisNode->getX() - cNode->getX();
					float yDist = thisNode->getY() - cNode->getY();
					float hScore = sqrt(pow(xDist, 2) + pow(yDist, 2)) + nodeList->at(listSlot).HScore;

					float xLeft = thisNode->getX() - goalNode->getX();
					float yLeft = thisNode->getY() - goalNode->getY();
					float gScore = sqrt(pow(xLeft, 2) + pow(yLeft, 2));

					//Add it to the list
					listElement tempNode;
				
					tempNode.node = node;
					tempNode.parent = listSlot;
					
					tempNode.HScore = hScore;
					tempNode.FScore = hScore;

					//Opening the node
					tempNode.state = openState;

					//Add the new node to the list
					nodeList->push_back(tempNode);
				}
			//}
		}

		//Close the current node
		nodeList->at(listSlot).state = closedState;

		//Checking if the goal node has been added to the open list
		for(unsigned int i = 0; i < nodeList->size(); i++)
		{
			if(nodeList->at(i).node == goalNode->getID() && nodeList->at(i).state == closedState) //The goal has been added to the closed list
			{
				pathFound = true;

				finalNode = i; //SAving the location of the final node
			}
		}
	}
	
	if(finalNode == -1)
	{
		return false;
	}
	else
	{
		bool pathSaved = false;
		
		//Selecting the last node
		int thisNode = finalNode;

		//adding the node to the path deque
		path->push_front(nodeList->at(thisNode).node);
		while(pathSaved == false)
		{
			thisNode = nodeList->at(thisNode).parent;

			if(thisNode == -1) //If this is the final node
			{
				pathSaved = true;
			}
			else
			{
				//add the node to the path deque
				path->push_front(nodeList->at(thisNode).node);
			}
		}
	}
	//Everything is done, the path has been saved properly

	//Removing garbage
	nodeList->clear();
	delete nodeList;
	return true;
}

/*
NodeLink NPC::findClosestNodes(World* world)
{
	float xOrigin = chr.getFeetX();
	float yOrigin = chr.getFeetY();

	float lowestDist = 100000000;

	NodeLink closestLink;
	closestLink.setNode(0, -1); //node[0] = -1;
	closestLink.setNode(1, -1); //node[1] = -1;

	int nodeAmount = world->getNodeAmount();

	//Going through all the nodes
	for(unsigned int i = 0; i < nodeAmount; i++)
	{
		PathNode* node = world->findNodeById(i);

		float xPos = node->getX();
		float yPos = node->getY();

		//Going thru all of the links
		for(unsigned int n = 0; n < node->getLinkAmount(); n++)
		{
			//Getting the second node
			PathNode* linkNode = world->findNodeById( node->getLinkID(n) );
			
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

					closestLink.node[0] = node->getID();
					closestLink.node[1] = linkNode->getID();
				}
			}
		}
	}

	if(lowestDist > 2) //If no suitable node was found
	{
		closestLink.node[0] = -1;
		closestLink.node[1] = -1;
	}

	agk::Print(lowestDist);

	return closestLink;
}
*/
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

	//Preventing collisioin between this and other characters
	agk::SetSpriteCategoryBit(colSID, 1, 0);
	agk::SetSpriteCategoryBit(colSID, GF_charGroup, 1);
	agk::SetSpriteCollideBit(colSID, GF_charGroup, 0);

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

	//Making sure that the left/right speed is not to big
	float chkSpeed = 15.0f;
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
}
void Character::walkRight()
{
	float moveForce = 30.0f;
	agk::SetSpritePhysicsForce(colSID, x, y, moveForce, 0);
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