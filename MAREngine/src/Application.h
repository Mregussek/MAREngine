/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

// Vertices
#include "Renderer/Mesh/Shapes/Shapes.h"
#include "Renderer/Mesh/Shapes/Cube.h"
// Rendering
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/RendererOpenGLFactory.h"
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
		char portName[10] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";

	public:
		Application() = default;

		// --- MAIN METHOD --- //
		int run();
	};
}