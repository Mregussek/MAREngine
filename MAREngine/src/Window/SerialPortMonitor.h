
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "../mre.h"

class SerialPortMonitor {
    char* _port;
    static const int _maxLength{255};
    char _incomingData[_maxLength]{};
    std::string _recvData;
    SerialPort* _arduino;
    std::thread _thread;
    std::mutex _mutex;
    float _x;
    float _y;
    float _z;
    int _connectTries;

public:
    SerialPortMonitor(char* portName, int tries = 10) 
        : _port(portName),
        _x(0.0f),
        _y(0.0f),
        _z(0.0f),
        _connectTries(tries)
    {
        _arduino = new SerialPort(_port);
    }

    ~SerialPortMonitor() { 
        delete _arduino; 

        if (_thread.joinable()) _thread.join();
    }

    void start() {
        int checkTries{ 0 };
        std::cout << "Searching for device on " << _port << " in progress";
        
        while (!_arduino->isConnected()) {
            Sleep(100);
            std::cout << ".";
            delete _arduino;
            _arduino = new SerialPort(_port);

            if (_connectTries < ++checkTries) {
                std::cout << "Cannot connect to SP, playing without SerialPort\n";
                return;
            }
        }

        if (_arduino->isConnected()) std::cout << "\nConnection established!\n"; \

        _thread = std::thread([this] {
            while (_arduino->isConnected()) receive();

            restart();
        });
    }

    const float& getX() const { 
        static std::mutex _io_mutex;
        std::lock_guard<std::mutex> lg(_io_mutex);
        return _x;
    }

    const float& getY() const {
        static std::mutex _io_mutex;
        std::lock_guard<std::mutex> lg(_io_mutex);
        return _y;
    }

    const float& getZ() const {
        static std::mutex _io_mutex;
        std::lock_guard<std::mutex> lg(_io_mutex);
        return _z;
    }

    const bool& isConnected() const { return _arduino->isConnected(); }

private:
    void receive() {
        int readResult = _arduino->readSerialPort(_incomingData, MAX_DATA_LENGTH);
        _recvData = _incomingData;
        parseInput();
        Sleep(125); 
    }

    void parseInput() {
        if (_recvData.find("#x#") != std::string::npos) {
            auto begin = _recvData.find(" ");
            auto end = _recvData.substr(begin + 1, _recvData.size() - 1).find(" ");

            _x = std::stof(_recvData.substr(begin + 1, end));
        }
        else if (_recvData.find("#y#") != std::string::npos) {
            auto begin = _recvData.find(" ");
            auto end = _recvData.substr(begin + 1, _recvData.size() - 1).find(" ");

            _y = std::stof(_recvData.substr(begin + 1, end));
        }
        else if (_recvData.find("#z#") != std::string::npos) {
            auto begin = _recvData.find(" ");
            auto end = _recvData.substr(begin + 1, _recvData.size() - 1).find(" ");

            _z = std::stof(_recvData.substr(begin + 1, end));
        }
    }

    void restart() {
        if (_thread.joinable()) _thread.join();
        start();
    }
};

#endif // !SERIALPORT_H