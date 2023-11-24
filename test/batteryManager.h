#pragma once
#include <vector>
#include <string>
#include "modbusMaster.h"
#include "battery.h"

class batteryManager
{
public:
	batteryManager();
	void chargeBatterys();
	void dischargeBatterys();
	void checkBattery(std::string address);
	void initialize();

	// это надо заприватить и сделать сетеры и гетеры
	std::vector<std::tuple<int, battery, modbusMaster>> chargeQueue;
	std::vector<std::tuple<int, battery, modbusMaster>> dischargeQueue;

	boolean stopProgramm;
private:
	// tut massiv a dolschna bit ochered
	//std::vector<int> chargeQueue;
	//std::vector<int> dischargeQueue;
};

