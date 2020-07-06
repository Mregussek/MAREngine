/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * 
 *	Description:
 *	This file contains all the include libraries for this engine, so that everything used to create
 *	something will be here. Also it defines 'mar' namespace with all its classes.
 */


//! ---- ACTUAL INCLUDES ---- !// 
// --- Import GLEW lib statically --- //
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// --- Include OpenGL Libs --- //
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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

#include "stb_image/stb_image.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include "SerialPort.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#pragma warning (pop)

// --- Include C++ STL libraries --- //
// I/O events
#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <vector> 
#include <utility>
#include <unordered_map>
#include <algorithm> // std::find, std::copy
// multi-threading
#include <thread> 
#include <mutex>
// random numbers
#include <random>
// filesystem
#include <filesystem>


/*!
	mar is a main namespace for MAREngine. Below you can find all classes,
	which are written for proper work of MAREngine. The most needed class is Window,
	cause it initializes OpenGL stuff. 
*/
namespace mar {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	struct TexturePaths;

	namespace filesystem {
		class fnc;
	}

	namespace window {
		/* namespace callbacks in Window.h */

		class Input;
		class Window;
	}

	namespace devices {
		class SerialPortMonitor;
	}

	namespace gui {
		struct GUIData;
		class GUI;
	}

	namespace layers {
		class Layer;
		class GUILayer;
		class MeshLayer;
		class CameraLayer;
		class LayerStack;
	}

	namespace graphics {
		struct CameraData;
		class Camera;

		namespace objects {
			class ObjectLoader;
		}

		struct RendererStatistics;
		class Renderer;
		class RendererFactory;
		class RendererOpenGLFactory;

		class Mesh;
		class MeshCreator;
		class Light;
		class Scene;

		class Shape;
		class ShapeManipulator;
		class Cube;
		class Pyramid;
		class Surface;
		class Wall;

		class VertexBuffer;
		class VertexBufferOpenGL;
		class VertexArray;
		class VertexArrayOpenGL;
		class VertexBufferLayout;
		class VertexBufferLayoutOpenGL;
		class ElementBuffer;
		class ElementBufferOpenGL;
		class FragmentBuffer;
		class FragmentBufferOpenGL;
		class Texture;
		class TextureOpenGL;
		class Shader;
		class ShaderOpenGL;
	}

	namespace engine {
		class MAREngine;
	}

#ifndef MAR_ENGINE_SETTINGS
#define MAR_ENGINE_SETTINGS

	namespace settings {
		inline const char name[10] = "MAREngine";
		inline const int width{ 1500 };
		inline const int height{ 900 };
		inline const char portName[10] = "\\\\.\\COM7";
		inline const char* glsl_version = "#version 330";
	}

#endif // !MAR_ENGINE_SETTINGS


#ifndef MAR_ENGINE_CONSTANTS
#define MAR_ENGINE_CONSTANTS

	namespace constants {
		inline const size_t maxCount = 100000;
		inline const size_t maxVertexCount = maxCount * 50;
		inline const size_t maxIndexCount = maxCount * 50;
		inline const size_t maxObjectsInScene = 32;
	}

#endif // !MAR_ENGINE_CONSTANTS


#ifndef MAR_ENGINE_STORAGE
#define MAR_ENGINE_STORAGE

	namespace storage {
		inline Ref<graphics::RendererFactory> factory;
		inline bool usegui;
	}

#endif // !MAR_ENGINE_STORAGE


}