/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


// ----------------------------------------------
// MAR_ENGINE_COMPILE_SETUP
// ----------------------------------------------

 // Comment if release mode
#define MAR_ENGINE_DEBUG_MODE

// Comment if logs should be in logs/log.txt file instead of printed on the terminal
//#define MAR_ENGINE_TERMINAL_LOGS

// Uncomment to see Editor logs
//#define MAR_ENGINE_EDITOR_LOGS

// Uncomment to see Graphics logs
#define MAR_ENGINE_GRAPHICS_LOGS

// Uncomment to see Entity Component System logs
#define MAR_ENGINE_ECS_LOGS

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
#pragma warning( disable : 26439 )
#pragma warning( disable : 26495) 
#pragma warning( disable : 26812) 
#pragma warning( disable : 26451) 
#pragma warning( disable : 6385) 
#pragma warning( disable : 6011) 
#pragma warning( disable : 6262) 
#pragma warning( disable : 6308) 
#pragma warning( disable : 4005) 
#pragma warning( disable : 4286) 
#pragma warning( disable : 4099) 

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

#if __has_include("SerialPort.hpp")
	#include "SerialPort.hpp"
	#define MAR_ENGINE_SERIAL_PORT_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import SerialPort.hpp!"
#endif

#if __has_include("spdlog/spdlog.h")
	#include "spdlog/spdlog.h"
	#include "spdlog/sinks/stdout_color_sinks.h"
	#include "spdlog/sinks/basic_file_sink.h"
	#define MAR_ENGINE_SPDLOG_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import spdlog/spdlog.h!"
#endif

#if __has_include("entt/entt.hpp")
	#include "entt/entt.hpp"
	#define MAR_ENGINE_ENTT_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import entt/entt.hpp!"
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

