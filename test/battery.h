#pragma once
class battery
{
public:
	battery();

	bool operator==(const battery& other) const;
	bool operator!=(const battery& other) const;

	void setVoltage(int voltage);
	int getVoltage();
	//void setAmperage(int amperage);
	//int getAmperage();
	void setCapacity(int capacity);
	int getCapacity();
private:
	int voltage;
	//int amperage;
	int capacity;

};

