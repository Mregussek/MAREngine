/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * 
 *	Description:
 *	This file contains all the include libraries for this engine, so that everything used to create
 *	something will be here. Also it defines 'mar' namespace with all its classes.
 */


// --- Import GLEW lib statically --- //
#define GLEW_STATIC

// Decide wheter you want import GLEW or GLAD (comment to use GLAD)
#define IMPORT_GLEW

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
#pragma warning( disable : 6385) 
#pragma warning( disable : 6011) 
#pragma warning( disable : 6262) 
#pragma warning( disable : 6308) 
#pragma warning( disable : 4005) 

#include "../other/stb_image/stb_image.h"
#include "SerialPort/SerialPort.h"
#include "../other/imgui/imgui.h"
#include "../other/imgui/imgui_impl_glfw.h"
#include "../other/imgui/imgui_impl_opengl3.h"

#pragma warning (pop)

// --- Include C++ STL libraries --- //
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>

namespace mar {
	class Camera;
	class Window;
	class GUI;
	class SerialPortMonitor;
	class Mesh;
	class VertexBuffer;
	class VertexArray;
	class VertexBufferLayout;
	class Texture;
	class Shader;
	class Renderer;
	class ElementBuffer;
	// cannot declare here namespace callbacks;
}