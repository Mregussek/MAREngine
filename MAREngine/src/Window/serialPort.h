
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "SerialPortMonitor/SerialPort.h"
#include "../mre.h"

class SerialPortMonitor {
    char* _port;
    static const int _maxLength{255};
    char _incomingData[_maxLength]{};
    SerialPort* _arduino;
    std::string _recvData;
    std::thread _thread;

public:
    SerialPortMonitor(char* portName) 
        : _port(portName),
        _recvData(std::string())
    {
        _arduino = new SerialPort(_port);
    }

    ~SerialPortMonitor() { 
        delete _arduino; 
    }

    void start() {
        std::cout << "Searching for device on " << _port << " in progress";
        while (!_arduino->isConnected()) {
            Sleep(100);
            std::cout << ".";
            _arduino = new SerialPort(_port);
        }

        if (_arduino->isConnected()) std::cout << "\nConnection established!\n";

        _thread = std::thread([this] {
            while(_arduino->isConnected()) receive(); 
        });
    }

    const std::string& receiveFromSerialPort() const { return _recvData; }
    const bool& isConnected() const { return _arduino->isConnected(); }

private:
    void receive() {
        int readResult = _arduino->readSerialPort(_incomingData, MAX_DATA_LENGTH);
        _recvData = _incomingData;
        printf("%s", _incomingData);
        Sleep(10);
    }
};

#endif // !SERIALPORT_H