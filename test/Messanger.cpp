#include "Messanger.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>

Messanger::Messanger() {}

std::string Messanger::getMessageFromPipe() {

    char errorMessage[] = {"Error"};

    HANDLE hPipe;
    const char* pipeName = "\\\\.\\pipe\\MyNamedPipe"; // Уникальное имя канала

    hPipe = CreateNamedPipeA(
        pipeName,                       // Имя канала
        PIPE_ACCESS_DUPLEX,             // Права доступа
        PIPE_TYPE_MESSAGE |             // Режим сообщений
        PIPE_READMODE_MESSAGE,          // Режим чтения сообщений
        PIPE_UNLIMITED_INSTANCES,       // Количество экземпляров (неограничено)
        0,                              // Размер выходного буфера (0 для системного размера)
        0,                              // Размер входного буфера (0 для системного размера)
        0,                              // Таймаут для операций (0 для бесконечного таймаута)
        NULL                            // Структура безопасности (NULL означает использование стандартной защиты)
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
    const char* pipeName = "\\\\.\\pipe\\MyNamedPipe"; // Уникальное имя канала

    hPipe = CreateFileA(
        pipeName,                    // Имя канала
        GENERIC_WRITE,               // Права доступа
        0,                           // Флаги и атрибуты
        NULL,                        // Структура защиты (NULL означает использование стандартной защиты)
        OPEN_EXISTING,               // Открываем существующий канал
        0,                           // Флаги асинхронного режима (0 для синхронного режима)
        NULL                         // Дескриптор шаблона файла (не используется)
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

