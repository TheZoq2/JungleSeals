#pragma once

#include <string>
#include "agk.h"

class DataEditor
{
public:
	DataEditor(void);
	~DataEditor(void);

	static std::string createData(std::string type, std::string value);
	static std::string createData(std::string type, int value);
	static std::string createData(std::string type, float value);
};

