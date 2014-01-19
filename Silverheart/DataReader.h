#pragma once

#include "agk.h"

class DataReader
{
public:
	DataReader(void);
	~DataReader(void);

	static uString getType(uString line);
	static uString getValue(uString line);
	static uString getValue(uString line, int pos);
	static int getValueAmount(uString line);
};

