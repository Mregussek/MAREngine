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

            static const char* GetGLErrorStr(GLenum err) {
                switch (err) {
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

            static void CheckGLError(const char* file, int line) {
                while (true) {
                    GLenum err = glGetError();
                    if (GL_NO_ERROR == err)
                        return;

                    getCoreLogger()->error(GetGLErrorStr(err));
                    getCoreLogger()->error(file);
                    getCoreLogger()->error(line);
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

#define MAR_CORE_CHECK_FOR_ERROR()  ::mar::debug::Log::CheckGLError(__FILE__, __LINE__) 

#define MAR_TRACE(...) ::mar::debug::Log::getClientLogger()->trace(__VA_ARGS__)
#define MAR_INFO(...)  ::mar::debug::Log::getClientLogger()->info(__VA_ARGS__)
#define MAR_WARN(...)  ::mar::debug::Log::getClientLogger()->warn(__VA_ARGS__)
#define MAR_ERROR(...) ::mar::debug::Log::getClientLogger()->error(__VA_ARGS__)

#endif // !MAR_ENGINE_LOG_H
