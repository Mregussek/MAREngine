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


#include "mar_includes.h"


// MAREngine Logging level
#define MARENGINE_LOGGING_LEVEL 3           // 0 - no logs, 1 - trace, 2 - debug, 3 - info, 4 - warn, 5 - err, 6 - critical
#define MARENGINE_DISPLAY_EDITOR_LOGS 0     // 0 - no logs, 1 - display editor logs
#define MARENGINE_DISPLAY_GRAPHICS_LOGS 1   // 0 - no logs, 1 - display graphics logs
#define MARENGINE_DISPLAY_ECS_LOGS 1        // 0 - no logs, 1 - display ecs logs
#define MARENGINE_DISPLAY_SCRIPTS_LOGS 0    // 0 - no logs, 1 - display scripts logs
#define MARENGINE_DISPLAY_FILESYSTEM_LOGS 1 // 0 - no logs, 1 - display filesystem logs
#define MARENGINE_DISPLAY_LAYERS_LOGS 0     // 0 - no logs, 1 - display layers logs
#define MARENGINE_DISPLAY_WINDOW_LOGS 0     // 0 - no logs, 1 - display window logs
#define MARENGINE_DISPLAY_PLATFORMS_LOGS 1  // 0 - no logs, 1 - display platforms logs

// MAREngine window library
#define MARENGINE_USE_GLFW_WINDOW 1
#define MARENGINE_USE_SDL_WINDOW 0
// MAREngine RenderAPI
#define MARENGINE_USE_OPENGL_RENDERAPI 1

#define MAR_NO_DISCARD [[nodiscard]]


#include "mar.inl"
