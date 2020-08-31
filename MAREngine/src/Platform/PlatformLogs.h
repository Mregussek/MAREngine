/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_PLATFORM_LOGGING_H
#define MAR_ENGINE_PLATFORM_LOGGING_H

#include "../Debug/Log.h"

#ifdef MAR_ENGINE_PLATFORM_LOGS

#define PLATFORM_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define PLATFORM_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define PLATFORM_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define PLATFORM_WARN(...) MAR_CORE_WARN(__VA_ARGS__)
#define PLATFORM_GL_FUNC(x) MAR_CORE_GL_FUNC(x)

#else

#define PLATFORM_TRACE(...)
#define PLATFORM_INFO(...) 
#define PLATFORM_ERROR(...)
#define PLATFORM_WARN(...) 
#define PLATFORM_GL_FUNC(x) x

#endif

#endif
