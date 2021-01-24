/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


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
