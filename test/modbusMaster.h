#pragma once
#include <string>

extern "C" {
#include <modbus.h>
}

class modbusMaster
{
public:
	modbusMaster();
	modbusMaster(std::string adress);
	void setConnection();
	void readRegisters(int start, int end);
	void writeRegister(int adress, std::string value);
	void disconnectFromSlave();

	bool operator==(const modbusMaster& other) const;
	bool operator!=(const modbusMaster& other) const;

	void writeResultToFile();//не здесь

	void chargeBattery();// ne zdes
	void dischargeBattery();// toze ne zdes

	uint16_t* getTabReg();
	void setTabReg(uint16_t tabRegVal[32]);
	int getRes();
	void setAdress(std::string adress);
	std::string getAdress();
	void setPort(int port);
	int getPort();
private:
	std::string adress;
	int port;// ne nuschno
	uint16_t tabReg[32]; //ne nuschno navernoe
	modbus_t* mb;
	int res; // ne nuschno
};

