#include "batteryManager.h"
#include "documentManager.h"
#include <thread>
#include <chrono>
#include <ctime>
#include <iostream>

batteryManager::batteryManager() {
	initialize();
}

void batteryManager::chargeBatterys() {
	while (!stopProgramm) {
		unsigned int startTime = clock();
		
		documentManager docManager;
		std::string fileName;

		for (int i = 0; i < chargeQueue.size(); i++) {
			//std::tuple<int, battery, modbusMaster> cellParam : chargeQueue
			//std::pair<battery, modbusMaster> pair
			//auto& [product, price]
			//pair.first.setVoltage(pair.second.getTabReg())

			int state;
			battery batt;
			modbusMaster batteryModbus;
			std::tie(state, batt, batteryModbus) = chargeQueue[i];

			fileName = "battery_" + batteryModbus.getAdress() + ".txt";

			batteryModbus.readRegisters(0, 13);
			uint16_t* readedRegs = batteryModbus.getTabReg();

			if (readedRegs[4] < 4100 && readedRegs[0] == 0) {
				batteryModbus.writeRegister(0, "1");

				docManager.writeToFile(fileName, "Start charging");

				std::cout << "Start charging\n"; // checking
			}
			else if (readedRegs[4] > 4100 && (readedRegs[0] == 1 || readedRegs[0] == 0)) {
				batteryModbus.writeRegister(0, "0");

				auto it = std::remove(chargeQueue.begin(), chargeQueue.end(), chargeQueue[i]); // here error c2678
				chargeQueue.erase(it, chargeQueue.end());

				if (state <= 1) dischargeQueue.push_back({ (state + 1), batt, batteryModbus });

				docManager.writeToFile(fileName, "End of charging");

				std::cout << "End of charging\n"; // checking
			}

			std::string strToWrite = std::to_string(readedRegs[0]) + "; " + std::to_string(readedRegs[2]) + "; "
				+ std::to_string(readedRegs[4]) + "; " + std::to_string(readedRegs[6]) + "; " +
				std::to_string(readedRegs[8]) + "; " + std::to_string(readedRegs[10]) + "; " +
				std::to_string(readedRegs[12]);

			docManager.writeToFile(fileName, strToWrite);

			std::cout << strToWrite << std::endl; // checking

		}

		unsigned int processingTime = (clock() - startTime)/1000.0;
		std::this_thread::sleep_for(std::chrono::nanoseconds((4 - processingTime) * 1000000000));
	}

	for (int i = 0; i < chargeQueue.size(); i++) std::get<2>(chargeQueue[i]).disconnectFromSlave();

}

void batteryManager::dischargeBatterys() {
	while (!stopProgramm) {
		unsigned int startTime = clock();

		documentManager docManager;
		std::string fileName;

		for (int i = 0; i < dischargeQueue.size(); i ++) {
			//std::tuple<int, battery, modbusMaster> cellParam : chargeQueue
			//std::pair<battery, modbusMaster> pair
			//auto& [product, price]
			//pair.first.setVoltage(pair.second.getTabReg())

			int state;
			battery batt;
			modbusMaster batteryModbus;
			std::tie(state, batt, batteryModbus) = dischargeQueue[i];

			fileName = "battery_" + batteryModbus.getAdress() + ".txt";

			batteryModbus.readRegisters(0, 13);
			uint16_t* readedRegs = batteryModbus.getTabReg();
			
			if (readedRegs[4] > 4100 && readedRegs[0] == 0) {
				batteryModbus.writeRegister(0, "2");

				docManager.writeToFile(fileName, "Start discharging");

				std::cout << "Start discharging\n"; // checking
			}
			else if (readedRegs[4] < 3600 && (readedRegs[0] == 2 || readedRegs[0] == 0)) {
				batteryModbus.writeRegister(0, "0");

				auto it = std::remove(dischargeQueue.begin(), dischargeQueue.end(), dischargeQueue[i]);
				dischargeQueue.erase(it, dischargeQueue.end());

				if(state <= 1) chargeQueue.push_back({ (state+1), batt, batteryModbus });

				docManager.writeToFile(fileName, "End of discharging");

				std::cout << "End of discharging\n"; // checking
			}

			std::string strToWrite = std::to_string(readedRegs[0]) + "; " + std::to_string(readedRegs[2]) + "; "
				+ std::to_string(readedRegs[4]) + "; " + std::to_string(readedRegs[6]) + "; " +
				std::to_string(readedRegs[8]) + "; " + std::to_string(readedRegs[10]) + "; " +
				std::to_string(readedRegs[12]);

			docManager.writeToFile(fileName, strToWrite);

			std::cout << strToWrite << std::endl; // checking

		}

		unsigned int processingTime = (clock() - startTime) / 1000.0;
		std::this_thread::sleep_for(std::chrono::nanoseconds((4 - processingTime) * 1000000000));
	}

	for (int i = 0; i < dischargeQueue.size(); i++) std::get<2>(dischargeQueue[i]).disconnectFromSlave();

}

void batteryManager::checkBattery(std::string address) {

	modbusMaster batteryModbus = modbusMaster(address);
	batteryModbus.setConnection();
	batteryModbus.writeRegister(0, "0");

	battery batt = battery();
	//{0 ,batt, batteryModbus }
	chargeQueue.push_back(std::make_tuple(0, batt, batteryModbus));
	//chargeBatterys();

	//std::pair<battery, modbusMaster> pair = { batt, batteryModbus };
	//std::tuple<int, battery, modbusMaster> myTuple = std::make_tuple(42, batt, batteryModbus);
	// Доступ к элементам tuple по именам
	/*int firstElement;
	double secondElement;
	std::string thirdElement;*/
	//std::tie(firstElement, secondElement, thirdElement) = myTuple;
}

void batteryManager::initialize() {
	stopProgramm = false;
}
