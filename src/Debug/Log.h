/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LOG_H
#define MAR_ENGINE_LOG_H

#include "../mar.h"

#ifdef MAR_ENGINE_DEBUG_MODE

namespace mar {
	namespace debug {

		class Log {
			static bool s_initialized;
			static Ref<spdlog::logger> s_CoreLogger;

		public:
			static void init();

            static std::string GetGLErrorStr(GLenum err) {
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

            static void chernoClear() {
                while (glGetError() != GL_NO_ERROR);
            }

            static bool chernoCheck(const char* function, const char* file, int line) {
                while (GLenum err = glGetError()) {
                    getCoreLogger()->error("[OpenGL Error] " + GetGLErrorStr(err));
                    getCoreLogger()->error(std::string(function) + " - " + std::string(file) 
                        + " - " + std::to_string(line));
                    return false;
                }

                return true;
            }

			inline static spdlog::logger* getCoreLogger() { return s_CoreLogger.get(); }
		};

		
} }


    #define MAR_LOG_INIT() ::mar::debug::Log::init()
    
    #define MAR_CORE_TRACE(...) ::mar::debug::Log::getCoreLogger()->trace(__VA_ARGS__)
    #define MAR_CORE_INFO(...)  ::mar::debug::Log::getCoreLogger()->info(__VA_ARGS__)
    #define MAR_CORE_WARN(...)  ::mar::debug::Log::getCoreLogger()->warn(__VA_ARGS__)
    #define MAR_CORE_ERROR(...) ::mar::debug::Log::getCoreLogger()->error(__VA_ARGS__)
    
    #define MAR_CORE_CHECK_FOR_ERROR()  ::mar::debug::Log::CheckGLError(__FILE__, __LINE__) 

    #define ASSERT(x, msg) if(!(x)) MAR_CORE_ERROR(msg);\
                           if(!(x)) __debugbreak()
                           
    #define MAR_CORE_ASSERT(x, msg) ASSERT(x, msg)
    
    #define MAR_CORE_GL_FUNC(x) ::mar::debug::Log::chernoClear();\
                                x;\
                                ASSERT(::mar::debug::Log::chernoCheck(#x, __FILE__, __LINE__), "OPENGL")

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

#endif // !MAR_ENGINE_LOG_H
