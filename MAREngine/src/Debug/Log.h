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


#ifdef MAR_ENGINE_DEBUG_MODE


namespace marengine {


	class Log {

		static bool s_initialized;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;

	public:

		static void init();

		static std::string GetGLErrorStr(GLenum err);

		static void CheckGLError(const char* file, int line);

		static void clearError();

		static bool checkForOpenGLError(const char* function, const char* file, int line);

		static spdlog::logger* getCoreLogger();

	};


}


#define MAR_LOG_INIT() ::marengine::Log::init()

#define MAR_CORE_TRACE(...) ::marengine::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MAR_CORE_DEBUG(...) ::marengine::Log::getCoreLogger()->debug(__VA_ARGS__)
#define MAR_CORE_INFO(...)  ::marengine::Log::getCoreLogger()->info(__VA_ARGS__)
#define MAR_CORE_WARN(...)  ::marengine::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MAR_CORE_ERROR(...) ::marengine::Log::getCoreLogger()->error(__VA_ARGS__)
#define MAR_CORE_CRITICAL(...) ::marengine::Log::getCoreLogger()->critical(__VA_ARGS__)

#define MAR_CORE_CHECK_FOR_ERROR()  ::marengine::Log::CheckGLError(__FILE__, __LINE__) 

#define ASSERT_NO_MSG(x) if(!(x)) __debugbreak()

#define ASSERT(x, ...) if(!(x)) MAR_CORE_ERROR(__VA_ARGS__);\
                       ASSERT_NO_MSG(x)

#define MAR_CORE_ASSERT(x, ...) ASSERT(x, __VA_ARGS__)

#define MAR_CORE_GL_FUNC(x) ::marengine::Log::clearError();\
                            x;\
                            ASSERT_NO_MSG(::marengine::Log::checkForOpenGLError(#x, __FILE__, __LINE__))

#else

#define ASSERT(...)
#define MAR_CORE_ASSERT(...)

#define MAR_LOG_INIT()

#define MAR_CORE_TRACE(...) 
#define MAR_CORE_INFO(...)  
#define MAR_CORE_WARN(...)  
#define MAR_CORE_ERROR(...) 

#define MAR_CORE_CHECK_FOR_ERROR()  

#define MAR_CORE_GL_FUNC(x) x;

#endif

#endif // !MAR_ENGINE_DEBUG_LOG_H
