#include "NPC.h"

#include "DebugConsole.h"

char NPC_path[128] = "Data/Characters/";

NPC::NPC(void)
{
}


NPC::~NPC(void)
{
}

void NPC::setup()
{
	formal = new std::vector< uString >;
}
void NPC::update(World* world)
{
	if(exists == true) //Making sure that the NPC exists before doing anything with it
	{
		chr.update(world); //Updating the character

		this->x = chr.getX();
		this->y = chr.getY();
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Character::create(uString colSprite)
{
	this->imgID = agk::LoadImage(colSprite);

	this->SID = agk::CreateSprite(imgID);

	//Temporary scale
	colScale = .04f;
	agk::SetSpriteScale(SID, colScale, colScale);

	//Setting physics properties
	agk::SetSpritePhysicsOn(SID, 2);

	agk::SetSpriteShape(SID, 3);
	agk::SetSpritePhysicsFriction(SID, 1.0f);
	agk::SetSpritePhysicsRestitution(SID, 0);
	agk::SetSpritePhysicsMass(SID, 0.1f);
	agk::SetSpriteDepth(SID, 15);

	jumpHeight = 3;
}
void Character::update(World* world)
{
	//Preventing the collision sprite from falling over
	agk::SetSpriteAngle(SID, 0);

	isOnGround = checkOnGround(world);

	//Updating the position of the sprite
	x = agk::GetSpriteXByOffset(SID);
	y = agk::GetSpriteYByOffset(SID);
}

void Character::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;

	agk::SetSpritePosition(SID, x, y);
}

float Character::getX()
{
	return x;
}
float Character::getY()
{
	return y;
}

void Character::jump()
{
	if(isOnGround && globaltime > lastJump + 0.1) //Checking if the player can jump
	{
		agk::SetSpritePhysicsImpulse(SID, x, y, 0, -jumpHeight);

		lastJump = globaltime;
	}
}

bool Character::checkOnGround(World* world)
{
	bool canJump = false;
	//Calculating the height of the sprite
	float plrWidth = agk::GetImageWidth(imgID) * colScale;
	float plrHeight = agk::GetImageHeight(imgID) * colScale;

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