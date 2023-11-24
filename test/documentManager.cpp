#include "documentManager.h"

documentManager::documentManager() {}

void documentManager::createFile(std::string portName) {
	std::ofstream fout;

	// получение текущего времени в виде строки
	/*std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%y_%H-%M-%S", now);*/

	std::string fileName = "battery_" + portName +".txt";
	fout.open(fileName);
	fout << "Battery at port " + portName;
	fout.close();
}
void documentManager::writeToFile(std::string fileName, std::string strToWrtite) {
	std::ofstream fout;
	fout.open(fileName);

	fout << std::endl << strToWrtite;

	fout.close();
}