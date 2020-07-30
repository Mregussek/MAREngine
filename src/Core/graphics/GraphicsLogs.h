/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_LOGGING_H
#define MAR_ENGINE_GRAPHICS_LOGGING_H

#include "../../Debug/Log.h"

#ifdef MAR_ENGINE_GRAPHICS_LOGS

#define GRAPHICS_TRACE(x) MAR_CORE_TRACE(x)
#define GRAPHICS_INFO(x) MAR_CORE_INFO(x)
#define GRAPHICS_ERROR(x) MAR_CORE_ERROR(x)
#define GRAPHICS_WARN(x) MAR_CORE_WARN(x)

#else

#define GRAPHICS_TRACE(...)
#define GRAPHICS_INFO(...) 
#define GRAPHICS_ERROR(...)
#define GRAPHICS_WARN(...) 

#endif

#endif

