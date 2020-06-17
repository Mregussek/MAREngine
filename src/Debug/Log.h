/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LOG_H
#define MAR_ENGINE_LOG_H

#include "../mar.h"

namespace mar {
	namespace debug {

		class Log {
			static bool s_initialized;
			static Ref<spdlog::logger> s_CoreLogger;
			static Ref<spdlog::logger> s_ClientLogger;

		public:
			static void init();

            static void glCheckError_(const char* file, int line) {
                GLenum errorCode;
                while ((errorCode = glGetError()) != GL_NO_ERROR) {
                    std::string error;
                    switch (errorCode) {
                    case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
                    case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
                    case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
                    case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
                    case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
                    case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
                    }

                    s_CoreLogger->error(error + " | " + file + " (" + std::to_string(line) + ")");
                }
            }

			inline static Ref<spdlog::logger>& getCoreLogger() { return s_CoreLogger; }
			inline static Ref<spdlog::logger>& getClientLogger() { return s_ClientLogger; }
		};

		
} }

#define MAR_LOG_INIT() ::mar::debug::Log::init()

#define MAR_CORE_TRACE(...) ::mar::debug::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MAR_CORE_INFO(...)  ::mar::debug::Log::getCoreLogger()->info(__VA_ARGS__)
#define MAR_CORE_WARN(...)  ::mar::debug::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MAR_CORE_ERROR(...) ::mar::debug::Log::getCoreLogger()->error(__VA_ARGS__)

#define MAR_CORE_CHECK_FOR_ERROR()  ::mar::debug::Log::glCheckError_(__FILE__, __LINE__) 

#define MAR_TRACE(...) ::mar::debug::Log::getClientLogger()->trace(__VA_ARGS__)
#define MAR_INFO(...)  ::mar::debug::Log::getClientLogger()->info(__VA_ARGS__)
#define MAR_WARN(...)  ::mar::debug::Log::getClientLogger()->warn(__VA_ARGS__)
#define MAR_ERROR(...) ::mar::debug::Log::getClientLogger()->error(__VA_ARGS__)

#endif // !MAR_ENGINE_LOG_H
