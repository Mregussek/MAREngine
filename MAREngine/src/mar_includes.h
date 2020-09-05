/**
 *				MAREngine - open source 3D game engine
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
// MAR_ENGINE_INCLUDE_LIBRARIES
// ----------------------------------------------

// Solve std::min / std::max error on windows
#ifndef NOMINMAX
	#define NOMINMAX
#endif

// --- Include OpenGL Libs --- //
#if __has_include(<GL/glew.h>)
	#ifndef GLEW_STATIC
		#define GLEW_STATIC
	#endif
	#include <GL/glew.h> 
	#define MAR_ENGINE_GLEW_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import GL/glew.h!"
#endif

#if __has_include(<GLFW/glfw3.h>)
	#include <GLFW/glfw3.h>
	#define MAR_ENGINE_GLFW_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import GLFW/glfw3.h!"
#endif

// --- Include other third-party libraries --- //
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

#if __has_include("stb_image/stb_image.h")
	#include "stb_image/stb_image.h"
	#define MAR_ENGINE_STB_IMAGE_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import stb_image/stb_image.h!"
#endif

#if __has_include("imgui.h")
	#include "imgui.h"
	#include "imgui_internal.h"
	#include "examples/imgui_impl_glfw.h"
	#include "examples/imgui_impl_opengl3.h"
	#define MAR_ENGINE_IMGUI_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import imgui.h!"
#endif

#if __has_include("ImGuizmo.h")
	#include "ImGuizmo.h"
	#define MAR_ENGINE_IMGUIZMO_LIB
#else
	#error "MAR ENGINE: Cannot import ImGuizmo.h!"
#endif

#if __has_include("TextEditor.h")
	#include "TextEditor.h"
	#define MAR_ENGINE_IMGUI_COLOR_TEXT_EDIT_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import ImGuiColorTextEdit lib : TextEditor.h !"
#endif

#if __has_include("FileBrowser/ImGuiFileBrowser.h")
	#include "FileBrowser/ImGuiFileBrowser.h"
	#define MAR_ENGINE_IMGUI_ADDONS_FILE_BROWSER_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import ImGui-Addons lib : FileBrowser/ImGuiFileBrowser.h !"
#endif

#if __has_include("spdlog/spdlog.h")
	#include "spdlog/spdlog.h"
	#include "spdlog/sinks/stdout_color_sinks.h"
	#include "spdlog/sinks/rotating_file_sink.h"
	#include "spdlog/sinks/basic_file_sink.h"
	#define MAR_ENGINE_SPDLOG_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import spdlog/spdlog.h!"
#endif

#if __has_include("Source/OBJ_Loader.h")
	#include "Source/OBJ_Loader.h"
	#define MAR_ENGINE_OBJ_LOADER_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import OBJ_Loader lib - Source/OBJ_Loader.h!"
#endif

#if __has_include("entt/entt.hpp")
	#include "entt/entt.hpp"
	#define MAR_ENGINE_ENTT_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import entt/entt.hpp!"
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
	#define MAR_ENGINE_MAR_MATHS_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import MARMaths.h!"
#endif

// --- OS Specific libraries --- //
#ifdef WIN32
	#if __has_include(<crtdbg.h>)
		#include <crtdbg.h>
	#else	
		#error "MAR ENGINE: Cannot import crtdbg.h for Windows Platform!"
	#endif
#endif

// --- Include C++ STL libraries --- //
// I/O events
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <vector> 
#include <utility>
#include <optional>
#include <variant>
#include <unordered_map>
#include <algorithm> // std::find, std::copy
#include <ctime>
// multi-threading
#include <thread> 
#include <mutex>
// random numbers
#include <random>
// filesystem
#include <filesystem>

