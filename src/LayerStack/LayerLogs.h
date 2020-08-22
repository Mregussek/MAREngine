/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_LOGGING_H
#define MAR_ENGINE_LAYER_LOGGING_H

#include "../Debug/Log.h"

#ifdef MAR_ENGINE_LAYER_LOGS

#define LAYER_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define LAYER_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define LAYER_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define LAYER_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define LAYER_TRACE(...)
#define LAYER_INFO(...) 
#define LAYER_ERROR(...)
#define LAYER_WARN(...) 

#endif

#endif
