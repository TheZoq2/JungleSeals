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


void NPC::createFromName(uString name)
{
	setup();
	//Getting the filename for the character
	uString filename;
	filename.Append(NPC_path);
	filename.Append(name);

	if(agk::GetFileExists(filename))
	{
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

					}
				}
			}

			delete[] p; //Removing the string

			line++;
		}
		agk::CloseFile(fileID);
	}
	else
	{
		//Telling the console that the file didin't exist
		DebugConsole::addC("Failed to load character: ");DebugConsole::addC(name);DebugConsole::addC("  ---  ");DebugConsole::addToLog(filename);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NPCGroup::setup()
{
	npc = new std::vector< NPC >;
}

void NPCGroup::addNPCFromFile(uString file)
{
	NPC tempNPC;
	tempNPC.createFromName(file);

	//Adding the NPC to the vector of NPCs
	npc->push_back(tempNPC);
}