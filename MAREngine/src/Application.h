/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

// Vertices
#include "Mesh/VertexBuffer/VertexBuffer.h"
#include "Mesh/VertexBuffer/VertexBufferLayout.h"
#include "Mesh/ElementBuffer/ElementBuffer.h"
#include "Mesh/VertexArray/VertexArray.h"
#include "Mesh/Shapes/Shapes.h"
// Rendering
#include "Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"
#include "Camera/Camera.h"
// Window && Input
#include "Window/Window.h"
#include "GUI/GUI.h"
#include "Devices/SerialPortMonitor.h"

namespace mar {

	class Application {
		char name[10] = "MAREngine";
		int width{ 1280 };
		int height{ 720 };
		const std::string shadersPath = "resources/shaders/basic.shader";
		const std::string texturePath = "resources/textures/mr.jpg";
		char portName[10] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";

	public:
		Application() = default;

		// --- MAIN METHOD --- //
		int run();
	};
}