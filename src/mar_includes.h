/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


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
#define MAR_ENGINE_DEBUG_MODE

// set logging level
#define MAR_ENGINE_LOGS_LEVEL_TRACE
//#define MAR_ENGINE_LOGS_LEVEL_INFO
//#define MAR_ENGINE_LOGS_LEVEL_WARN
//#define MAR_ENGINE_LOGS_LEVEL_ERROR

/*
!!! Comment if logs should be in logs/log.txt file instead of printed on the terminal

Normally MAREngine was developed with printed on terminal logs, in order to catch something red (such errors).
Also it is needed to store somewhere loggings (let's say in release mode). I saw that, logging to file was not 
affecting actual perfomance of engine so much, so I created another option here. Leave uncommented in order to see
every log on terminal.

If MAR_ENGINE_DEBUG_MODE is commented, MAR_ENGINE_TERMINAL_LOGS will not affect anything.
*/
#define MAR_ENGINE_TERMINAL_LOGS


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

#if __has_include("SerialPort.hpp")
	#include "SerialPort.hpp"
	#define MAR_ENGINE_SERIAL_PORT_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import SerialPort.hpp!"
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

