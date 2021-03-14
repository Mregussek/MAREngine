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


// ----------------------------------------------
// MAR_ENGINE_INCLUDE_LIBRARIES
// ----------------------------------------------

// Solve std::min / std::max error on windows
#ifndef NOMINMAX
	#define NOMINMAX
#endif

// --- OS Specific libraries --- //
#if defined(_WIN32) || defined(WIN32)
    #if __has_include(<crtdbg.h>)
		#include <crtdbg.h>
	#else	
		#error "MAR ENGINE: Cannot import crtdbg.h for Windows Platform!"
    #endif
    #if __has_include(<windows.h>)
        #include <windows.h>
    #else
        #error "MAR ENGINE: Cannot import windows.h for Windows Platform!"
    #endif
#endif
#if defined(__unix__) || defined(linux)
    #error "MARENGINE does not support linux yet!"
#endif

// --- Include 3rd_party libs --- //

#pragma warning( push )
#pragma warning( disable : 26439	)
#pragma warning( disable : 26495	) 
#pragma warning( disable : 26812	) 
#pragma warning( disable : 26451	) 
#pragma warning( disable : 6385		) 
#pragma warning( disable : 6011		) 
#pragma warning( disable : 6262		) 
#pragma warning( disable : 6308		) 
#pragma warning( disable : 4005		) 
#pragma warning( disable : 4018		)
#pragma warning( disable : 4286		) 
#pragma warning( disable : 4244		) 
#pragma warning( disable : 4099		) 

#if __has_include(<glad/glad.h>)
	#include <glad/glad.h>
#else
	#error "MAR ENGINE: Cannot import glad/glad.h!"
#endif

#if __has_include(<SDL.h>)
	#define HAVE_M_PI
	#include <SDL.h>
#else
	#error "MAR ENGINE: Cannot import SDL2.h!"
#endif

#if __has_include(<GLFW/glfw3.h>)
	#include <GLFW/glfw3.h>
#else
	#error "MAR ENGINE: Cannot import GLFW/glfw3.h!"
#endif

#if __has_include("stb_image/stb_image.h")
	#include "stb_image/stb_image.h"
#else
	#error "MAR ENGINE: Cannot import stb_image/stb_image.h!"
#endif

#if __has_include("imgui.h")
	#include "imgui.h"
	#include "imgui_internal.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "backends/imgui_impl_sdl.h"
#else
	#error "MAR ENGINE: Cannot import imgui.h!"
#endif

#if __has_include("ImGuizmo.h")
	#include "ImGuizmo.h"
#else
	#error "MAR ENGINE: Cannot import ImGuizmo.h!"
#endif

#if __has_include("TextEditor.h")
	#include "TextEditor.h"
#else
	#error "MAR ENGINE: Cannot import ImGuiColorTextEdit lib : TextEditor.h !"
#endif

#if __has_include("FileBrowser/ImGuiFileBrowser.h")
	#include "FileBrowser/ImGuiFileBrowser.h"
#else
	#error "MAR ENGINE: Cannot import ImGuiFileBrowser lib : FileBrowser/ImGuiFileBrowser.h !"
#endif

#if __has_include("spdlog/spdlog.h")
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
    #define SPDLOG_DEBUG_ON
    #define SPDLOG_TRACE_ON
	#include "spdlog/spdlog.h"
	#include "spdlog/sinks/stdout_color_sinks.h"
	#include "spdlog/sinks/rotating_file_sink.h"
	#include "spdlog/sinks/basic_file_sink.h"
#else
	#error "MAR ENGINE: Cannot import spdlog/spdlog.h!"
#endif

#if __has_include("entt/entt.hpp")
	#include "entt/entt.hpp"
#else
	#error "MAR ENGINE: Cannot import entt/entt.hpp!"
#endif

#if __has_include("json.hpp")
	#include "json.hpp"
#else
	#error "MAR ENGINE: Cannot import json.hpp!"
#endif

#if __has_include(<pybind11/pybind11.h>)
	#include <pybind11/pybind11.h>
	#include <pybind11/embed.h>
#else
	#error "MARMathPythonModule: Cannot import pybind11/pybind11.h!"
#endif

#pragma warning (pop)

// --- Include MAR libraries --- //

#if __has_include("MARMaths.h")
	#include "MARMaths.h"
#else
	#error "MAR ENGINE: Cannot import MARMaths.h!"
#endif

// --- Include C++ STL libraries --- //

// I/O events
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <vector> 
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <variant>
#include <random>
#include <filesystem>
#include <type_traits>

