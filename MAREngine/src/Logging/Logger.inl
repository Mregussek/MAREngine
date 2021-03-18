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


#ifndef MARENGINE_LOGGER_INL
#define MARENGINE_LOGGER_INL


#include "Logger.h"


namespace marengine {


    template<ELoggerType TLoggerType>
    constexpr bool checkIfCanBeLogged() {
        if constexpr (TLoggerType == ELoggerType::EDITOR && MARENGINE_DISPLAY_EDITOR_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::ECS && MARENGINE_DISPLAY_ECS_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::GRAPHICS && MARENGINE_DISPLAY_GRAPHICS_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::SCRIPTS && MARENGINE_DISPLAY_SCRIPTS_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::FILESYSTEM && MARENGINE_DISPLAY_FILESYSTEM_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::PLATFORMS && MARENGINE_DISPLAY_PLATFORMS_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::WINDOW && MARENGINE_DISPLAY_WINDOW_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::LAYERS && MARENGINE_DISPLAY_LAYERS_LOGS) {
            return true;
        }
        if constexpr (TLoggerType == ELoggerType::NORMAL) {
            return true;
        }

        return false;
    }

}


#if MARENGINE_LOGGING_LEVEL

#define MARLOG_TRACE(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_TRACE(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                                 \
}
#define MARLOG_DEBUG(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_DEBUG(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                                 \
}
#define MARLOG_INFO(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_INFO(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                                 \
}
#define MARLOG_WARN(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_WARN(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                                 \
}
#define MARLOG_ERR(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_ERROR(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                               \
}
#define MARLOG_CRIT(LoggerType, msg, ...) if constexpr (::marengine::checkIfCanBeLogged<LoggerType>()) { \
SPDLOG_LOGGER_CRITICAL(::marengine::FLogger::getLogger(), msg, __VA_ARGS__);                             \
}


// not using ::marengine::FLogger, because I want to throw error, when below code is used outside MAREngine's code.
#define PLATFORM_GL_FUNC(function)   FLogger::clearErrorOpenGL();                              \
                                     function;                                                 \
                                     FLogger::callDebugBreak(FLogger::checkErrorOpenGL());

#define MAR_ASSERT(function, ...)    const bool issueOccurred{ !(function) }; \
                                     if(issueOccurred) {                      \
                                        FLogger::err(__VA_ARGS__);            \
                                        FLogger::callDebugBreak(true);        \
                                     }

#else

// if no loggging enabled, we want to define macros but not check anything
#define PLATFORM_GL_FUNC(function)
#define MAR_ASSERT(function, ...)

#endif


#endif //MARENGINE_LOGGER_INL
