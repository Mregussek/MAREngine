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


    enum class ELoggerType {
        EDITOR, GRAPHICS, ECS, SCRIPTS, FILESYSTEM, PLATFORMS,
        WINDOW, LAYERS
    };


    class Logger {
    public:

        static void init();

        template<ELoggerType TLoggerType, typename... TArgs>
        static void trace(std::string message, TArgs&&... args);

        template<ELoggerType TLoggerType, typename... TArgs>
        static void debug(std::string message, TArgs&&... args);

        template<ELoggerType TLoggerType, typename... TArgs>
        static void info(std::string message, TArgs&&... args);

        template<ELoggerType TLoggerType, typename... TArgs>
        static void warn(std::string message, TArgs&&... args);

        template<ELoggerType TLoggerType, typename... TArgs>
        static void err(std::string message, TArgs&&... args);

        template<ELoggerType TLoggerType, typename... TArgs>
        static void critic(std::string message, TArgs&&... args);

        static void clearErrorOpenGL();

        static bool checkErrorOpenGL(const char* function, const char* file, int line);

        static const char* getOccuredErrorOpenGl(GLenum issue);

    private:

        template<ELoggerType TLoggerType>
        static bool checkIfCanBeLogged();

        static std::shared_ptr<spdlog::logger> s_logger;

    };


}


#include "Logger.inl"


#endif // !MAR_ENGINE_DEBUG_LOG_H
