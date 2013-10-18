#include "Script.h"

#include "DebugConsole.h"

Script::Script(void)
{
}


Script::~Script(void)
{
}

void Script::run(uString scriptName, Part* part, World* world, Player* player)
{
	std::vector<char*> lines;

	char* p; //Pointer to allow us to remove read lines

	//Reading the file
	int fileID = agk::OpenToRead(scriptName.GetStr());
		p = agk::ReadLine(fileID); //Because there is some nonsense at the begining of the script, we need to read one line first. This makes the first line of the script the second line of the file
		delete[] p;
		p = agk::ReadLine(fileID); //The first line is always the amount of lines in the script
		int lineAmount = agk::Val(p);
		delete[] p; //Deleting the string

		for(int i = 0; i < lineAmount; i++)
		{
			lines.push_back(agk::ReadLine(fileID));
		}
	agk::CloseFile(fileID); //Closing the file

	//Execute the script
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		runFunction(lines[i], part, world, player);
	}

	//We are done reading the script, remove all the pointers
	for(unsigned int i = 0; i < lines.size(); i++)
	{
		delete[] lines[i];
	}
}

void Script::runFunction(uString function, Part* part, World* world, Player* player)
{
	//Passing the function to the complete function list
	runFunction(function, part, world, player, NULL);
}
void Script::runFunction(uString function, Part* part, World* world, Player* player, NPCGroup* npcGroup)
{
	//Separating the command from the parameters
	uString fcommand;
		
	//fcommand.SetStr(lines[i]);
	fcommand.SetStr(function);

	//Removing all of the spaces
	fcommand.ReplaceStr(" ","");

	int commandLength = fcommand.FindStr("(");

	//Separating the command from the line
	uString command;
	fcommand.SubString(command, 0, commandLength); 
		
	if(command.CompareTo("playerGoTo") == 0)
	{
		//This command requires one parameter
		int paramEnd = fcommand.FindStr(")");

		//uString param; //The parameter for what world to enter
		uString entrance; //The parameter for where the player should start in the new level

		// fcommand.SubString(param, commandLength + 1, paramEnd - commandLength - 1);

		uString levelName;

		levelName.SetStr("levels/");
		levelName.Append(getParam(fcommand, 1, false, part));
		entrance.SetStr(getParam(fcommand, 2, true, part));

		//Working out what level we are going to

		//We have the world we want to go to, let's check if it exists
		if(player->canTravel())
		{
			world->clear(); //Clear this world to make room for a new one
			world->load(levelName); //Loading the new level
			player->spawn(entrance); //Respawning the player in this new world

			DebugConsole::addC("Traveling to ");
			DebugConsole::addC(levelName);
			DebugConsole::addC(" Entrance: ");
			DebugConsole::addToLog(entrance);

			//Making sure we don't change worlds to often
			player->setJustLoaded(true);
		}
	}

	//				playerSpawnAt
	if(command.CompareTo("playerSpawnAt") == 0)
	{
		uString param;
			
		//int paramEnd = fcommand.FindStr(")");

		//fcommand.SubString(param, commandLength + 1, paramEnd - commandLength - 1);
		param.SetStr(getParam(fcommand, 1, true, part));

		//Telling the player to spawn at the part
		player->spawn(param);
	}

	if(command.CompareTo("setPhysOn") == 0) //This changes the physics of the part to dynamic
	{
		part->setPhysState(2); //Turning on physics
	}

	if(command.CompareTo("turn") == 0)
	{
		//Getting the amount to turn the part
		float amount = agk::ValFloat(getParam(fcommand, 1, true, part));

		agk::SetSpritePhysicsAngularVelocity(part->getSID(), amount);
	}
	
	if(command.CompareTo("turnFromPlr") == 0) //Turns the item away from the player with the force supplied as parameter
	{
		//Getting the amount to turn the part
		float amount = agk::ValFloat(getParam(fcommand, 1, true, part));
		
		//Calculating the direction of the force
		float plrX = player->getX();
		float partX = part->getX();
		float diff = partX - plrX;
		if(diff < 0)
		{
			//The tree should be turned the other way
			amount = amount * -1;
		}
		agk::SetSpritePhysicsAngularVelocity(part->getSID(), amount);
	}

	if(command.CompareTo("setUnusable") == 0)
	{
		part->setUsable(0);
	}

	if(command.CompareTo("playerMove") == 0)
	{
		Part* targetPart = world->getPartFromName(getParam(fcommand, 1, true, part));
		player->spawn(getParam(fcommand, 1, true, part)); 
		//player->setPosition(targetPart->getX(), targetPart->getY());
	}

	if(command.CompareTo("openShop") == 0)
	{
		//Opening the shop
		IngameMenu::createShop(player, 1);
	}
	
	//Print command
	if(command.CompareTo("print") == 0)
	{
		DebugConsole::addToLog(getParam(fcommand, 1, true, NULL));
	}
	
	//////////////////////////////////////////////////////////////////
	//					World modifications
	//////////////////////////////////////////////////////////////////
	if(command.CompareTo("setOvercast") == 0)
	{
		//getting the parameter
		float overcast = float( atof(getParam(fcommand, 1, true, part)) );

		world->setOvercast(overcast);
	}
	if(command.CompareTo("setTime") == 0)
	{
		float time = float( atof(getParam(fcommand, 1, true, part)) );

		world->setTime(time);
	}
	if(command.CompareTo("getTime") == 0)
	{
		DebugConsole::addToLog(agk::Str(world->getTime()));
	}
	
	if(command.CompareTo("setPlayerVisible") == 0)
	{
		int visible = atoi(getParam(fcommand, 1, true, part));

		player->setVisible(visible);
	}

	if(command.CompareTo("loadNPC2") == 0)
	{
		npcGroup->addNPCFromFile(getParam(fcommand, 1, true, part));
	}

	if(command.CompareTo("loadNPC") == 0)
	{
		uString npc;
		npc.SetStr(getParam(fcommand, 1, false, part)); //Getting the name of the NPC to load

		uString partName;
		partName.SetStr(getParam(fcommand, 2, true, part)); //Getting the name of the part to check

		Part* spawnpoint = world->getPartFromName(partName);

		if(spawnpoint != NULL)
		{
			float xPos = spawnpoint->getX();
			float yPos = spawnpoint->getY();

			npcGroup->addNPCFromFile(npc, xPos, yPos);
		}
		else
		{
			DebugConsole::addC("The spawnpoint: ");
			DebugConsole::addC(partName);
			DebugConsole::addC(" for character ");
			DebugConsole::addC(npc);
			DebugConsole::addToLog(" was not found, character was not spawned");
		}
	}

	if(command.CompareTo("setPlayerWeapon") == 0)
	{
		player->setCurrentWeaponByName(getParam(fcommand, 1, true, part));
	}
}

/*void runFunction(uString function, World* world, Enemy* enemy)
{
	//Separating the command from the parameters
	uString fcommand;
		
	//fcommand.SetStr(lines[i]);
	fcommand.SetStr(function);

	//Removing all of the spaces
	fcommand.ReplaceStr(" ","");

	int commandLength = fcommand.FindStr("(");

	//Separating the command from the line
	uString command;
	fcommand.SubString(command, 0, commandLength);

		//Spawns enemies
	if(command.CompareTo("spawnE") == 0)
	{
		enemy->begin(world);
		enemy->load(GF::getPath("Bound.png"));
		enemy->spawn("entrance");

	}
}*/




uString Script::getParam(uString command, int number, bool isLast, Part* part)
{
	//Finding the end of the first command
	int cmdStart = 0; 
	int cmdEnd = command.FindStr("("); //Finding the start of the first command. This is given to the end command because they are swapped in the for loop

	uString restParams;
	restParams.SetStr(command);

	command.SubString(restParams, cmdEnd + 1); //Separating the parameters from the command

	uString result; //The final parameter, after label check
	uString param; //The parameter separated from the rest of the command
	for(int i = 0; i < number; i++)
	{
		cmdStart = 0; //cmdEnd;
		//Getting a substring with the remaining commands

		if(i == number - 1 && isLast == true) //This is the last parameter, look for the closing ")" instead of the ","
		{
			cmdEnd = restParams.FindStr(")"); //Finding the closing parameter
		}
		else
		{
			cmdEnd = restParams.FindStr(","); //Finding the ending parameter
		}

		if(i == number - 1) //Looks like we have found the parameter, let's separate it from the rest
		{
			restParams.SubString(param, cmdStart, cmdEnd);
		}

		restParams.SubString(restParams, cmdEnd + 1); //Removing the current parameter
	}

	if(isSpecial(param)) //Making sure this wasn't a special command
	{
		if(isLabel(param))
		{
			result.Append( getValueFromLabel(param, part) ); //Setting the level name to what the label says
		}
	}
	else //This was just a normal parameter
	{
		result.Append(param);
	}

	return result; //Return the result
}

bool Script::isSpecial(uString param)
{
	if(param.FindStr("#") != -1) //-1 because findStr returns the position
	{
		return true;
	}
	return false;
}

bool Script::isLabel(uString param)
{
	if(param.FindStr("#lbl") != -1)
	{
		return true;
	}
	return false;
}

uString Script::getValueFromLabel(uString param, Part* part)
{
	//Getting the number of the label
	uString number;
	param.SubString(number, param.GetLength() - 1);

	int val = agk::Val(number);

	uString value;
	value.SetStr(part->getLabel(val));

	return value;
}