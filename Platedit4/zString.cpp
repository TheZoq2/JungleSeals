#include "zString.h"


zString::zString(void)
{
}


zString::~zString(void)
{
}

std::vector< std::string > zString::split(std::string str, std::string splitChar)
{
	std::vector< std::string > result;

	//Finding the first splitchar
	int splitPos = str.find(splitChar);
	while(splitPos != -1)
	{
		result.push_back(str.substr(0, splitPos));
		str.erase(0, splitPos + 1);

		splitPos = str.find(splitChar);
	}
	result.push_back(str); //Saving the last line

	return result;
}