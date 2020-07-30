/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_LOGGING_H
#define MAR_ENGINE_EDITOR_LOGGING_H

#include "../Debug/Log.h"

#ifdef MAR_ENGINE_EDITOR_LOGS

#define EDITOR_TRACE(x) MAR_CORE_TRACE(x)
#define EDITOR_INFO(x) MAR_CORE_INFO(x)
#define EDITOR_ERROR(x) MAR_CORE_ERROR(x)
#define EDITOR_WARN(x) MAR_CORE_WARN(x)

#else

#define EDITOR_TRACE(...)
#define EDITOR_INFO(...) 
#define EDITOR_ERROR(...)
#define EDITOR_WARN(...) 

#endif

#endif
