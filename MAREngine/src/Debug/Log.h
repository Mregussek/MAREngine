/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_DEBUG_LOG_H
#define MAR_ENGINE_DEBUG_LOG_H


#include "../mar.h"


namespace marengine {


    class ILoggerType {
    public:

        virtual void create();

        template<typename TLogger, typename... Args>
        void trace(std::string message, Args&&... args);

        template<typename TLogger, typename... Args>
        void debug(std::string message, Args&&... args);

        template<typename TLogger, typename... Args>
        void info(std::string message, Args&&... args);

        template<typename TLogger, typename... Args>
        void warn(std::string message, Args&&... args);

        template<typename TLogger, typename... Args>
        void err(std::string message, Args&&... args);

        template<typename TLogger, typename... Args>
        void critic(std::string message, Args&&... args);

    };


    template<typename T>
    class LoggerType : public ILoggerType {
    public:

        void create() override;

        template<typename... Args>
        void trace(std::string message, Args&&... args);

        template<typename... Args>
        void debug(std::string message, Args&&... args);

        template<typename... Args>
        void info(std::string message, Args&&... args);

        template<typename... Args>
        void warn(std::string message, Args&&... args);

        template<typename... Args>
        void err(std::string message, Args&&... args);

        template<typename... Args>
        void critic(std::string message, Args&&... args);

    private:

        T m_loggerType;

    };


    class Logger {
    public:

        static void init(ILoggerType* loggerType);

        template<typename... Args>
        static void trace(std::string message, Args&&... args);

        template<typename... Args>
        static void debug(std::string message, Args&&... args);

        template<typename... Args>
        static void info(std::string message, Args&&... args);

        template<typename... Args>
        static void warn(std::string message, Args&&... args);

        template<typename... Args>
        static void err(std::string message, Args&&... args);

        template<typename... Args>
        static void critic(std::string message, Args&&... args);

        static void clearErrorOpenGL();

        static bool checkErrorOpenGL(const char* function, const char* file, int line);

        static const char* getOccuredErrorOpenGl(GLenum issue);

    private:

        static ILoggerType* s_loggerType;

    };


}


#include "Logger.inl"


#endif // !MAR_ENGINE_DEBUG_LOG_H
