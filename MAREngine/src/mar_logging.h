/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


// ----------------------------------------------
// MAR_ENGINE_COMPILE_SETUP
// ----------------------------------------------

/*
!!! Comment if release mode

Release Mode means here, that all code responsible for logging will not exist.
MAREngine is using macro-definitions for logging such as MAR_CORE_ERROR(x), which is replaced
by actual line of code. Commenting MAR_ENGINE_DEBUG_MODE means all those macro's will be just macro's.

Logging is damaging perfomance, so it will be wise to comment it (Of course if you don't need logging).
*/
#define MAR_ENGINE_DEBUG_MODE 1


/*
!!! Uncomment to see Editor logs

Uncommenting this macro will create ability to see logs from code, such as GUI editor, managing the whole things,
saving scene to file, load scene from file and others. In summary all things associated to Editor Layer.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_EDITOR_LOGS will not affect anything.
*/
//#define MAR_ENGINE_EDITOR_LOGS


/*
!!! Uncomment to see Graphics logs

Uncommenting this macro will create ability to see logs from code, such as Renderer API, the whole EBO, VBO stuff,
what happens during rendering, calling shaders, mesh creation and others.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_GRAPHICS_LOGS will not affect anything.
*/
#define MAR_ENGINE_GRAPHICS_LOGS


/*
!!! Uncomment to see Entity Component System logs

Uncommenting this macro will create ability to see logs from code, such as Entity Component System stuff. Specifically
entity creation, adding components to it, when components are called. In order to see it proper way, you should add
GRAPHICS_LOGS and EDITOR_LOGS, because ECS are the core of MAREngine. Entities && Components are called wherever needed.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_ECS_LOGS will not affect anything.
*/
#define MAR_ENGINE_ECS_LOGS

/*
!!! Uncomment to see Scripting Logs
*/
//#define MAR_ENGINE_SCRIPTING_LOGS

/*
!!! Uncomment to see Filesystem Logs
*/
//#define MAR_ENGINE_FILESYSTEM_LOGS


/*
!!! Uncomment to see Layer Logs

MAREngine is updating the whole frame with LayerStack, where each layer calls its overrided update() method. Sometimes
it is usefull to see, if at which point each layer is.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_LAYER_LOGS will not affect anything.
*/
//#define MAR_ENGINE_LAYER_LOGS

/*
!!! Uncomment to see Window Logs

Everything is displayed on window, so logging from window itself might be helpful. Uncommenting this macro
will let you catch, when input is enabled, which key is pressed or operations on window like cleaning, swapping buffers.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_WINDOW_LOGS will not affect anything.
*/
//#define MAR_ENGINE_WINDOW_LOGS

/*
!!! Uncomment to see Platform Logs
*/
#define MAR_ENGINE_PLATFORM_LOGS