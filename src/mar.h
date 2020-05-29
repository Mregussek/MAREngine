/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * 
 *	Description:
 *	This file contains all the include libraries for this engine, so that everything used to create
 *	something will be here. Also it defines 'mar' namespace with all its classes.
 */

 // --- Tell the compiler, if you imported repository from github
 // --- Leave it in comment, if you have the same setup as MR
 //#define IMPORTED_FROM_GITHUB

 // Decide whether you want import GLEW or GLAD (comment to use GLAD)
#define IMPORT_GLEW

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

// --- Include MAR third-party libraries --- //
#include "MARMaths/MARMaths.h"

// --- Include other third-party libraries --- //
#pragma warning( push )
#pragma warning( disable : 26495) 
#pragma warning( disable : 6385) 
#pragma warning( disable : 6011) 
#pragma warning( disable : 6262) 
#pragma warning( disable : 6308) 
#pragma warning( disable : 4005) 
#pragma warning( disable : 4286) 
#pragma warning( disable : 4099) 

#include "../other/stb_image/stb_image.h"
#include "SerialPort/SerialPort.h"
#include "../other/imgui/imgui.h"
#include "../other/imgui/imgui_impl_glfw.h"
#include "../other/imgui/imgui_impl_opengl3.h"

#pragma warning (pop)

// --- Include C++ STL libraries --- //
// I/O events
#include <iostream>
#include <fstream> 
#include <string>
// data structures
#include <vector> 
#include <unordered_map>
#include <tuple>
#include <algorithm> // std::find, std::copy
// multi-threading
#include <thread> 
#include <mutex>

//! mar
/*!
	mar is a main namespace for MAREngine. Below you can find all classes,
	which are written for proper work of MAREngine. The most needed class is Window,
	cause it initializes OpenGL stuff. 
*/
namespace mar {
	class Camera;
	class Window; 

	class SerialPortMonitor;
	class ObjectLoader;

	class GUI;
	struct GUIData;

	struct RendererStatistics;
	class Renderer;
	class RendererFactory;
	class RendererOpenGLFactory;

	class Mesh;
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
	class Texture;
	class TextureOpenGL;
	class Shader;
	class ShaderOpenGL;
	
	// and of course callbacks namespace in Window.h
	class MAREngine;
}