#pragma once

#include <string>
#include <vector>

class zString
{
public:
	zString(void);
	~zString(void);

	static std::vector< std::string > split(std::string str, std::string splitChar);
};

