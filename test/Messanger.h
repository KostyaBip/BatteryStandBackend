#pragma once
#include <string>
class Messanger
{
public:
	Messanger();
	std::string getMessageFromPipe();
	void sendMessageToPipe(std::string message);
};

