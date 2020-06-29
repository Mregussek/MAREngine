/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


#include "Debug/Log.h"
#include "FileSystem/FileSystem.h"
// Layers
#include "Layers/LayerStack.h"
#include "Layers/GUILayer.h"
#include "Layers/MeshLayer.h"
#include "Layers/CameraLayer.h"
// Rendering
#include "Renderer/Mesh/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include "Renderer/RendererFactory.h"
#include "Renderer/RendererOpenGLFactory.h"
#include "Renderer/Scene/Scene.h"
// Window && Input
#include "Window/Input.h"
#include "Window/Window.h"
#include "GUI/GUI.h"
#include "Devices/SerialPortMonitor.h"

namespace mar {
	namespace engine {


		struct MAREngineSettings {
			static const char name[10];
			static const int width;
			static const int height;
			static const char portName[10];
			static const char* glsl_version;
		};

		class MAREngine {
			window::Window m_window;

		public:
			MAREngine() = default;

			void run();
		};


} }

#endif // !MAR_ENGINE_MAIN_H