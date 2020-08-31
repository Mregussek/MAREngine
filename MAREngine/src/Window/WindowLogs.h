/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_WINDOW_LOGGING_H
#define MAR_ENGINE_WINDOW_LOGGING_H

#include "../Debug/Log.h"

#ifdef MAR_ENGINE_WINDOW_LOGS

#define WINDOW_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define WINDOW_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define WINDOW_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define WINDOW_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define WINDOW_TRACE(...)
#define WINDOW_INFO(...) 
#define WINDOW_ERROR(...)
#define WINDOW_WARN(...) 

#endif

#endif
