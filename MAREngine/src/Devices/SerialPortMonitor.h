/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "../mar.h"
#include "../Debug/Log.h"


namespace mar {
    namespace devices {


		/*!
            //! SerialPortMonitor
			SerialPortMonitor is used real-time reading data from Serial Port with specified port name.
			It looks for x, y, z-axis coordinates from accelerometer. For this we need to send data to serial
			port in pattern, which this is expecting.

			Pattern: "#x# <x-axis value [float]> #y# <y-axis value [float]> #z# <z-axis value [float]>"
			Pattern Example: "#x# 2.45 #y# -1.756 #z# 5.123"
		*/
        class SerialPortMonitor {
            char* _port;
            static const int _maxLength{ 255 };
            char _incomingData[_maxLength]{};
            std::string _recvData;
            SerialPort* _arduino;
            std::thread _thread;
            std::mutex _mutex;
            static const int _sleepTime{ 25 };
            float _x;
            float _y;
            float _z;
            int _connectTries;

        public:
            
            /*
            SerialPortMonitor constructor, it sets variables to its basic values
            and creates instance of arduino. This instance will look for connection on given portName

                \param portName - name of serial port, where device will be connected. Example: "\\\\.\\COM7"
                \param tries - how many tries SPM must check for connection, if device is not responding (default value is 10).
            */
            SerialPortMonitor(char* portName, int tries = 10);

            // SerialPortMonitor destructor. It deletes arduino instance and join thread.
            ~SerialPortMonitor();

            // Initialization process for SerialPortMonitor. Looks for connection on given portName/
            // If found, it starts a thread, which will read data
            void start();

            /*
            Get x-axis coordinate from accelerometer
            If cannot read default value is returned (0.0)

                \return _x - x-axis coordinate
            */
            const float& getX() const;

            /*
            Get y-axis coordinate from accelerometer
            If cannot read default value is returned (0.0)

                \return _y - y-axis coordinate
            */
            const float& getY() const;
 
            /*
            Get z-axis coordinate from accelerometer
            If cannot read default value is returned (0.0)

                \return _z - z-axis coordinate
            */
            const float& getZ() const;

            /*
            Check if device is still connected to Serial Port on given portName

                \param bool - true if is connected, false otherwise
            */
            const bool isConnected() const;

        private:
            /*
            Method written for receiving data in a thread. It continously reads
            data from Serial Port from device connected to given portName.
            */
            void receive();

            /*
            Method written for parsing input received from Serial Port. It expects
            specific pattern for x, y and z-axis coordinates. If the pattern is found
            it prescribes new values to _x, _y and _z member values.
            */
            void parseInput();
        };


} }

#endif // !SERIALPORT_H