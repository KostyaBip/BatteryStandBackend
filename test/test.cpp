#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include "Messanger.h"
#include "modbusMaster.h"
#include "batteryManager.h"

void initialize(batteryManager *battManager) {
    //batteryManager batterManager;
    battManager->initialize();
    std::thread myThread1(&batteryManager::chargeBatterys, &(*battManager));
    std::thread myThread2(&batteryManager::dischargeBatterys, &(*battManager));
}

int main()
{
    std::cout << "Start\n";

    Messanger messanger = Messanger();
    std::string adress =  messanger.getMessageFromPipe();

    std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
    messanger.sendMessageToPipe("Address recievd");

    batteryManager *battManager = new batteryManager();
    //initialize(battManager);
    battManager->initialize();
    std::thread myThread1(&batteryManager::chargeBatterys, &(*battManager));
    std::thread myThread2(&batteryManager::dischargeBatterys, &(*battManager));

    battManager->checkBattery(adress);

    std::cout << "Checking is started\n";

    std::string stopProgramm = messanger.getMessageFromPipe();
    while (stopProgramm != "true") {
        messanger.sendMessageToPipe("send again (true == stop)");
        stopProgramm = messanger.getMessageFromPipe();
    }

    messanger.sendMessageToPipe("interrupted");
    battManager->stopProgramm = true;

    std::this_thread::sleep_for(std::chrono::nanoseconds(5000000000));

    std::cout << "interrupted";
    myThread1.join();
    myThread2.join();

    /*
    modbusMaster client = modbusMaster(adress);
    client.setConnection();

    while (true) {

        client.readRegisters(4, 7);

        std::cout << "Result: " << client.getRes() << std::endl;
        uint16_t* tabReg = client.getTabReg();
        std::string resStr = "";
        for (int i = 0; i < client.getRes(); i++) {
            printf("reg[%d]=%d (0x%X)\n", i, tabReg[i], tabReg[i]);
            resStr += tabReg[i] + " ";
        }

        messanger.sendMessageToPipe(resStr);

        std::string mode = messanger.getMessageFromPipe();

        client.writeRegister(0, mode);
    }

    client.disconnectFromSlave();
    client.writeResultToFile();
    */
}

