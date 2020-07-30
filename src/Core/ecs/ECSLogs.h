/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_LOGGING_H
#define MAR_ENGINE_ECS_LOGGING_H

#include "../../Debug/Log.h"

#ifdef MAR_ENGINE_ECS_LOGS

#define ECS_TRACE(x) MAR_CORE_TRACE(x)
#define ECS_INFO(x) MAR_CORE_INFO(x)
#define ECS_ERROR(x) MAR_CORE_ERROR(x)
#define ECS_WARN(x) MAR_CORE_WARN(x)

#else

#define ECS_TRACE(...)
#define ECS_INFO(...) 
#define ECS_ERROR(...)
#define ECS_WARN(...) 

#endif

#endif
