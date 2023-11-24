#include "modbusMaster.h"
#include <fstream>
#include <ctime>
#include <iostream>

modbusMaster::modbusMaster(){}

modbusMaster::modbusMaster(std::string adress) {
	this->setAdress(adress);
	//this->setPort(port);
	uint16_t initRegValue[32];
	for (int i = 0; i < 32; i++) {
		initRegValue[i] = 1;
	}
	this->setTabReg(initRegValue);
}

void modbusMaster::setConnection() {
	//this->mb = modbus_new_tcp(this->getAdress().c_str(), this->getPort());
	this->mb = modbus_new_rtu(this->getAdress().c_str(), 115200, 'N', 8, 1);

	//std::cout << this->getAdress().c_str() << std::endl;

	if (mb == NULL) {
		std::cout << "Unable to create the libmodbus context\n";
		return;
	}

	if (modbus_connect(this->mb) == -1) {
		std::cout << "Connection failed: " << modbus_strerror(errno);
		modbus_free(this->mb);
		return;
	}

	modbus_set_slave(this->mb, 10);

}

void modbusMaster::readRegisters(int startAdress, int numOfBits) {
	this->res = modbus_read_input_registers(this->mb, startAdress, numOfBits, this->getTabReg());

}

void modbusMaster::writeRegister(int adress, std::string value) {
	uint16_t valueUint = (uint16_t)(std::stoi(value));
	modbus_write_register(mb, adress, valueUint);
}

void modbusMaster::disconnectFromSlave() {
	modbus_close(this->mb);
	modbus_free(this->mb);
}

void modbusMaster:: writeResultToFile() {
	std::ofstream fout;

	// получение текущего времени в виде строки
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%m-%d-%y_%H-%M-%S", now);

	std::string fileName = this->adress + "_" + std::to_string(this->port) + "_" + buffer + ".txt";
	fout.open(fileName);
	fout << this->getTabReg();
	fout.close();
}

void modbusMaster::chargeBattery() {

}

void modbusMaster::dischargeBattery() {

}

// set and get finctions //

uint16_t* modbusMaster::getTabReg() {
	return this->tabReg;
}

void modbusMaster::setTabReg(uint16_t tabRegVal[32]) {
	for (int i = 0; i < 32; i++) {
		tabReg[i] = tabRegVal[i];
	}
}

int modbusMaster::getRes() {
	return this->res;
}

void modbusMaster::setAdress(std::string adress) {
	this->adress = adress;
}

std::string modbusMaster::getAdress() {
	return this->adress;
}

void modbusMaster::setPort(int port) {
	this->port = port;
}

int modbusMaster::getPort() {
	return this->port;
}

bool modbusMaster::operator==(const modbusMaster& other) const {
	return this->adress == other.adress;
}

bool modbusMaster::operator!=(const modbusMaster& other) const {
	return !(*this == other);
}
