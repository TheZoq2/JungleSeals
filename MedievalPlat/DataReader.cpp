#include "DataReader.h"

#include "DebugConsole.h"

DataReader::DataReader(void)
{
}


DataReader::~DataReader(void)
{
}

uString DataReader::getType(uString line)
{
	uString type;

	//Checking if this is a comment
	uString comment;comment.SetStr("//");
	uString commentChk;
	line.SubString(commentChk, 0, 2);
	if(comment.CompareTo(commentChk) == 0) //This wasn't a comment, check for commands
	{
		type.SetStr("comment");
	}
	else //This was a comment
	{
		//Finding the : on the line
		int colon = line.Find(':');
		//Checking if the colon is actually there
		if(colon != -1)
		{
			line.SubString(type, 0, colon);
		}
		else
		{
			type.SetStr("error_typeNotFound");
		}
	}

	return type;
}

uString DataReader::getValue(uString line)
{
	uString value;
	//Finding the colon which is the start of the character
	int colon = line.Find(':');

	if(colon != -1) //Making sure there actually is a colon on  the line
	{
		line.SubString(value, colon + 1, -1); //Returning the whole line value
	}
	else
	{
		//Since a user friendly error is returned when finding the colon in the type function,
		//This function will add a simpler error to the log
		DebugConsole::addC("Colon not found when getting value from line: ");
		DebugConsole::addToLog(line);
	}

	return value;
}

int DataReader::getValueAmount(uString line)
{
	return line.Count(',');
}