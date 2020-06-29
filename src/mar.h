/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * 
 *	Description:
 *	This file contains all the include libraries for this engine, so that everything used to create
 *	something will be here. Also it defines 'mar' namespace with all its classes.
 */

// --- DECISIONS FOR USER 
// {
//
// --- Tell the compiler, if you imported repository from github
// --- Leave it in comment, if you have the same setup as MR 
//#define IMPORTED_FROM_GITHUB
//
// Decide whether you want import GLEW or GLAD (comment to use GLAD)
#define IMPORT_GLEW
//
// }


//! ---- ACTUAL INCLUDES ---- !// 
// --- Import GLEW lib statically --- //
#define GLEW_STATIC

// --- Include OpenGL Libs --- //
#ifdef IMPORT_GLEW
#include <GL/glew.h> 
#else
#include <glad/glad.h>
#endif

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
// data structures
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

#ifndef MAR_ENGINE_CONSTANTS
#define MAR_ENGINE_CONSTANTS

	namespace constants {
		const size_t maxCount = 100000;
		const size_t maxVertexCount = maxCount * 50;
		const size_t maxIndexCount = maxCount * 50;
		const size_t maxObjectsInScene = 32;
	}

#endif // !MAR_ENGINE_CONSTANTS

	struct TexturePaths;

	namespace window {
		/* namespace callbacks in Window.h */

		class Input;
		class Window;
	}

	namespace devices {
		class SerialPortMonitor;
	}

	namespace filesystem {
		class FileSystem;
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
}