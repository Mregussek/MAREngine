/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


 // Comment if release mode
//#define MAR_ENGINE_DEBUG_MODE


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
	#error
#endif

// --- Include other third-party libraries --- //
#pragma warning( push )
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
	#include "examples/imgui_impl_glfw.h"
	#include "examples/imgui_impl_opengl3.h"
	#define MAR_ENGINE_IMGUI_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import imgui.h!"
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
#include <unordered_map>
#include <algorithm> // std::find, std::copy
// multi-threading
#include <thread> 
#include <mutex>
// random numbers
#include <random>
// filesystem
#include <filesystem>

