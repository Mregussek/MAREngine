/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCRIPTING_LOGGING_H
#define MAR_ENGINE_SCRIPTING_LOGGING_H

#include "../../Debug/Log.h"

#ifdef MAR_ENGINE_SCRIPTING_LOGS

#define SCRIPTING_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define SCRIPTING_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define SCRIPTING_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define SCRIPTING_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define SCRIPTING_TRACE(...)
#define SCRIPTING_INFO(...) 
#define SCRIPTING_ERROR(...)
#define SCRIPTING_WARN(...) 

#endif

#endif
