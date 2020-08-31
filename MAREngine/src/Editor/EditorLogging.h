/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_LOGGING_H
#define MAR_ENGINE_EDITOR_LOGGING_H

#include "../Debug/Log.h"

#ifdef MAR_ENGINE_EDITOR_LOGS

#define EDITOR_TRACE(...) MAR_CORE_TRACE(__VA_ARGS__)
#define EDITOR_INFO(...) MAR_CORE_INFO(__VA_ARGS__)
#define EDITOR_ERROR(...) MAR_CORE_ERROR(__VA_ARGS__)
#define EDITOR_WARN(...) MAR_CORE_WARN(__VA_ARGS__)

#else

#define EDITOR_TRACE(...)
#define EDITOR_INFO(...) 
#define EDITOR_ERROR(...)
#define EDITOR_WARN(...) 

#endif

#endif
