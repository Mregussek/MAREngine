/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

// Rendering
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
		const std::string mrTex = "resources/textures/mr.jpg";
		const std::string yellowTex = "resources/textures/yellow-texture.jpg";
		const std::string grassTex = "resources/textures/grass-texture.jpg";
		const std::string wallTex = "resources/textures/wall.jpg";
		const std::string blueTex = "resources/textures/blue-texture.jpg";
		const std::string redTex = "resources/textures/red-texture.jpg";
		const std::string blackTex = "resources/textures/blac-texture.jpg";
		char portName[10] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";

	public:
		Application() = default;

		// --- MAIN METHOD --- //
		int run();
	};
}