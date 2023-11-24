#pragma once
#include <fstream>
class documentManager
{
public:
	documentManager();

	void createFile(std::string portName);
	void writeToFile(std::string fileName, std::string strToWrtite);
};

