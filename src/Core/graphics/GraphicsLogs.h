/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_LOGGING_H
#define MAR_ENGINE_GRAPHICS_LOGGING_H

#include "../../Debug/Log.h"

#ifdef MAR_ENGINE_GRAPHICS_LOGS

#define GRAPHICS_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define GRAPHICS_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define GRAPHICS_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define GRAPHICS_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define GRAPHICS_TRACE(...)
#define GRAPHICS_INFO(...) 
#define GRAPHICS_ERROR(...)
#define GRAPHICS_WARN(...) 

#endif

#endif

