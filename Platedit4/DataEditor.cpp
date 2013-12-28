#include "DataEditor.h"


DataEditor::DataEditor(void)
{
}


DataEditor::~DataEditor(void)
{
}

std::string DataEditor::createData(std::string type, std::string value)
{
	std::string result = type;
	result.append(":");

	result.append(value);

	return result;
}
std::string DataEditor::createData(std::string type, int value)
{
	std::string result = type;
	result.append(":");

	char* pVal;
	pVal = agk::Str(value);
	result.append(pVal);
	
	return result;
}
std::string DataEditor::createData(std::string type, float value)
{
	std::string result = type;
	result.append(":");

	char* pVal;
	pVal = agk::Str(value);
	result.append(pVal);
	
	return result;
}