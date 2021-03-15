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


#include "Logger.h"


namespace marengine {

    std::shared_ptr<spdlog::logger> FLogger::s_logger{ nullptr };


    void FLogger::init() {
        spdlog::set_pattern("%^%T:[%s:%#]:[%!]: %v%$");
        s_logger = spdlog::stdout_color_mt("MAREngine");

        if constexpr(MARENGINE_LOGGING_LEVEL == 0) {
            s_logger->set_level(spdlog::level::trace);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 1) {
            s_logger->set_level(spdlog::level::debug);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 2) {
            s_logger->set_level(spdlog::level::info);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 3) {
            s_logger->set_level(spdlog::level::warn);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 4) {
            s_logger->set_level(spdlog::level::err);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 5) {
            s_logger->set_level(spdlog::level::critical);
        }
        else if constexpr(MARENGINE_LOGGING_LEVEL == 6) {
            s_logger->set_level(spdlog::level::off);
        }
    }

    std::shared_ptr<spdlog::logger>& FLogger::getLogger() {
        return s_logger;
    }

    void FLogger::callDebugBreak(bool shouldCallDebugBreak) {
        if(shouldCallDebugBreak) {
            __debugbreak();
        }
    }

    void FLogger::clearErrorOpenGL() {
        while (glGetError() != GL_NO_ERROR);
    }

    bool FLogger::checkErrorOpenGL() {
        while (const GLenum issue = glGetError()) {
            return true;
        }

        return false;
    }

    const char* FLogger::getOccuredErrorOpenGl(GLenum issue) {
        switch (issue) {
            case GL_NO_ERROR:          return "No error";
            case GL_INVALID_ENUM:      return "Invalid enum";
            case GL_INVALID_VALUE:     return "Invalid value";
            case GL_INVALID_OPERATION: return "Invalid operation";
            case GL_STACK_OVERFLOW:    return "Stack overflow";
            case GL_STACK_UNDERFLOW:   return "Stack underflow";
            case GL_OUT_OF_MEMORY:     return "Out of memory";
            default:                   return "Unknown error";
        }
    }


}
