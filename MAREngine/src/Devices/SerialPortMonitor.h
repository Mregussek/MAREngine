/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "../mar.h"

namespace mar {
    class SerialPortMonitor {
        char* _port;
        static const int _maxLength{ 255 };
        char _incomingData[_maxLength]{};
        std::string _recvData;
        SerialPort* _arduino;
        std::thread _thread;
        std::mutex _mutex;
        static const int _sleepTime{25};
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
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
                });
        }

        const float& getX() const {
            static std::mutex _io_mutex;
            std::lock_guard<std::mutex> lg(_io_mutex);
            std::cout << "SPM _x: " << _x << std::endl;
            return _x;
        }

        const float& getY() const {
            static std::mutex _io_mutex;
            std::lock_guard<std::mutex> lg(_io_mutex);
            std::cout << "SPM _y: " << _y << std::endl;
            return _y;
        }

        const float& getZ() const {
            static std::mutex _io_mutex;
            std::lock_guard<std::mutex> lg(_io_mutex);
            std::cout << "SPM _z: " << _z << std::endl;
            return _z;
        }

        const bool isConnected() const { return _arduino->isConnected(); }

    private:
        void receive() {
            int readResult = _arduino->readSerialPort(_incomingData, MAX_DATA_LENGTH);
            _recvData = _incomingData;
            parseInput();
            std::this_thread::sleep_for(std::chrono::milliseconds(_sleepTime));
        }

        void parseInput() {
            try {
                auto xFound = _recvData.find("#x#");
                auto yFound = _recvData.find("#y#");
                auto zFound = _recvData.find("#z#");

                if (xFound == std::string::npos || yFound == std::string::npos || zFound == std::string::npos)
                    return;

                auto xBegin = xFound + 3;
                auto yBegin = yFound + 3;
                auto zBegin = zFound + 3;

                auto xEnd = yFound - 1;
                auto yEnd = zFound - 1;
                auto zEnd = std::distance(_recvData.begin(), _recvData.end()) - 2;

                _x = std::stof(_recvData.substr(xBegin, xEnd));
                _y = std::stof(_recvData.substr(yBegin, yEnd));
                _z = std::stof(_recvData.substr(zBegin, zEnd));
            }
            catch (std::exception& e) {
                std::cout << "Found error during parsing serial port: " << e.what() << std::endl;
            }
        }
    };
}

#endif // !SERIALPORT_H