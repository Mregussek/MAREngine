/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

// Rendering
#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/RendererOpenGLFactory.h"
#include "Renderer/Scene/Scene.h"
// Window && Input
#include "Window/Window.h"
#include "GUI/GUI.h"
#include "Devices/SerialPortMonitor.h"

namespace mar {

	class Application {
		char name[10] = "MAREngine";
		int width{ 1280 };
		int height{ 720 };
		char portName[10] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";

	public:
		Application() = default;

		// --- MAIN METHOD --- //
		int run();
	};
}