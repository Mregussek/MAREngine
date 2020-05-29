/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H

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
	namespace engine {


		struct {
			char name[10] = "MAREngine";
			int width{ 1280 };
			int height{ 720 };
			char portName[10] = "\\\\.\\COM7";
			const char* glsl_version = "#version 460";
		} MAREngineDefaultSettings;

		class MAREngine {
			window::Window m_window;
			gui::GUI m_gui;
			graphics::Camera m_camera;
			graphics::Renderer m_renderer;
			graphics::Scene m_scene;
			graphics::Mesh m_mesh;

		public:
			MAREngine() = default;

			void initialize();

			void run();

			void shutdown();
		};


} }

#endif // !MAR_ENGINE_MAIN_H