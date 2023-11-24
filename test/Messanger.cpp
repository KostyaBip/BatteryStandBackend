#include "Messanger.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>

Messanger::Messanger() {}

std::string Messanger::getMessageFromPipe() {

    char errorMessage[] = {"Error"};

    HANDLE hPipe;
    const char* pipeName = "\\\\.\\pipe\\MyNamedPipe"; // ���������� ��� ������

    hPipe = CreateNamedPipeA(
        pipeName,                       // ��� ������
        PIPE_ACCESS_DUPLEX,             // ����� �������
        PIPE_TYPE_MESSAGE |             // ����� ���������
        PIPE_READMODE_MESSAGE,          // ����� ������ ���������
        PIPE_UNLIMITED_INSTANCES,       // ���������� ����������� (������������)
        0,                              // ������ ��������� ������ (0 ��� ���������� �������)
        0,                              // ������ �������� ������ (0 ��� ���������� �������)
        0,                              // ������� ��� �������� (0 ��� ������������ ��������)
        NULL                            // ��������� ������������ (NULL �������� ������������� ����������� ������)
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create pipe. Error code: " << GetLastError() << std::endl;
        //return errorMessage;
    }

    std::cout << "Waiting for a connection..." << std::endl;

    BOOL result = ConnectNamedPipe(hPipe, NULL);
    if (!result && GetLastError() != ERROR_PIPE_CONNECTED) {
        std::cerr << "Failed to connect to pipe. Error code: " << GetLastError() << std::endl;
        CloseHandle(hPipe);
        //return errorMessage;
    }

    char buffer[256];
    DWORD bytesRead;

    result = ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL);

    if (!result) {
        std::cerr << "Failed to read from pipe. Error code: " << GetLastError() << std::endl;
    }
    else {
        buffer[bytesRead] = '\0';
        std::cout << "Message received: " << buffer << std::endl;
        //return buffer;
    }

    CloseHandle(hPipe);

    std::string str(buffer);
    return str;
}

void Messanger::sendMessageToPipe(std::string message) {
    HANDLE hPipe;
    const char* pipeName = "\\\\.\\pipe\\MyNamedPipe"; // ���������� ��� ������

    hPipe = CreateFileA(
        pipeName,                    // ��� ������
        GENERIC_WRITE,               // ����� �������
        0,                           // ����� � ��������
        NULL,                        // ��������� ������ (NULL �������� ������������� ����������� ������)
        OPEN_EXISTING,               // ��������� ������������ �����
        0,                           // ����� ������������ ������ (0 ��� ����������� ������)
        NULL                         // ���������� ������� ����� (�� ������������)
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open pipe. Error code: " << GetLastError() << std::endl;
        //return 1;
    }

    //std::string message = "Message from pipe!";
    DWORD bytesWritten;
    BOOL result = WriteFile(hPipe, message.c_str(), message.size(), &bytesWritten, NULL);

    if (!result) {
        std::cerr << "Failed to write to pipe. Error code: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "Message sent: " << message << std::endl;
    }

    CloseHandle(hPipe);
}

