#include "battery.h"

battery::battery() {
	this->voltage = -1;
	this->capacity = -1;
}

void battery::setVoltage(int voltage) {
	this->voltage = voltage;
}

int battery::getVoltage() {
	return this->voltage;
}

// Перегрузка оператора == как члена класса
bool battery::operator==(const battery& other) const {
	return this->voltage == other.voltage && this->capacity == other.capacity;
}

// Перегрузка оператора != как члена класса
bool battery::operator!=(const battery& other) const {
	return !(*this == other);
}

//void battery::setAmperage(int amperage) {
//	this->amperage = amperage;
//}

//int battery::getAmperage() {
//	return this->amperage;
//}

void battery::setCapacity(int capacity) {
	this->capacity = capacity;
}

int battery::getCapacity() {
	return this->capacity;
}
