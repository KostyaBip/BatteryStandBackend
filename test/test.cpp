// test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <iostream>
#include <stdio.h>
#include <string>
#include "Messanger.h"

extern "C" {
#include <modbus.h>
}


int main()
{
    Messanger readMessage = Messanger();
    readMessage.MessageFromPipe();
    
    std::cout << "Start\n";
    modbus_t* mb;
    uint16_t tab_reg[32];
    tab_reg[0] = 1;

    mb = modbus_new_tcp("127.0.0.1", 502);
    modbus_connect(mb);

    /* Read 5 registers from the address 0 */
    int res = modbus_read_registers(mb, 0, 5, tab_reg);
    std::cout << tab_reg[0] << std::endl << "Result: " << res;

    modbus_close(mb);
    modbus_free(mb);
}

