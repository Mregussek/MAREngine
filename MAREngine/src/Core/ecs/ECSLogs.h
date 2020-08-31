/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_LOGGING_H
#define MAR_ENGINE_ECS_LOGGING_H

#include "../../Debug/Log.h"

#ifdef MAR_ENGINE_ECS_LOGS

#define ECS_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define ECS_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define ECS_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define ECS_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define ECS_TRACE(...)
#define ECS_INFO(...) 
#define ECS_ERROR(...)
#define ECS_WARN(...) 

#endif

#endif
