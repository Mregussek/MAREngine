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

	struct {
		char name[10] = "MAREngine";
		int width{ 1280 };
		int height{ 720 };
		char portName[10] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";
	} MAREngineDefaultSettings;

	class MAREngine {
		Camera m_camera;
		Window m_window;
		GUI m_gui;
		Renderer m_renderer;
		Scene m_scene;

	public:
		MAREngine() = default;

		void initialize();

		void run();

		void shutdown();
	};
}