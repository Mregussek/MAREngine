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


#include "Log.h"


namespace marengine {

    ILoggerType* Logger::s_loggerType{ nullptr };


    void ILoggerType::create() {
        static_assert(true, "Cannot call ILoggerType::create()!!!");
    }


    void Logger::init(ILoggerType* loggerType) {
        s_loggerType = loggerType;
        s_loggerType->create();
    }


    template<>
    void LoggerType<std::shared_ptr<spdlog::logger>>::create() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
        m_loggerType = spdlog::stdout_color_mt("MAR ENGINE");
        m_loggerType->set_level(spdlog::level::warn);
	}

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::trace(std::string message, Args&&... args) {
        m_loggerType->template trace(std::move(message), std::forward<Args>(args)...);
    }

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::debug(std::string message, Args&&... args) {
        m_loggerType->template debug(std::move(message), std::forward<Args>(args)...);
    }

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::info(std::string message, Args&&... args) {
        m_loggerType->template info(std::move(message), std::forward<Args>(args)...);
    }

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::warn(std::string message, Args&&... args) {
        m_loggerType->template warn(std::move(message), std::forward<Args>(args)...);
    }

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::err(std::string message, Args&&... args) {
        m_loggerType->template error(std::move(message), std::forward<Args>(args)...);
    }

    template<> template<typename... Args>
    void LoggerType<std::shared_ptr<spdlog::logger>>::critic(std::string message, Args&&... args) {
        m_loggerType->template critical(std::move(message), std::forward<Args>(args)...);
    }


    void Logger::clearErrorOpenGL() {
        while (glGetError() != GL_NO_ERROR);
    }

    bool Logger::checkErrorOpenGL(const char* function, const char* file, int line) {
        while (GLenum issue = glGetError()) {
            //Logger::err(std::string("[OpenGL Error] {} {} {} \n{}"), getOccuredErrorOpenGl(issue), function, file, line);
            return false;
        }

        return true;
    }

    const char* Logger::getOccuredErrorOpenGl(GLenum issue) {
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
