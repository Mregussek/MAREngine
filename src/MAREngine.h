/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


// Log && Filesystem
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
			static MAREngine s_instance;

			window::Window m_window;
			std::string m_pathLoad{ "resources/mar_files/load_default.marscene" };

			Ref<graphics::RendererFactory> m_factory{ graphics::RendererOpenGLFactory::createFactory() };
			bool m_usegui{ true };
			bool m_shouldRestart{ false };

		public:
			MAREngine() = default;

			void initialize();
			void run();
			void exit();

			// --- SET METHODS --- //
			static MAREngine* getEngine() { return &s_instance; }

			void setLoadPath(std::string path) { m_pathLoad = path.c_str(); }
			void setRestart() { m_shouldRestart = true; }

		private:
			void assignLoadedLayers(layers::LayerStack* stack,
									layers::GUILayer* gui_layer,
									layers::CameraLayer* camera_layer,
									Ref<graphics::FrameBuffer>& framebuffer);
		};


} }

#endif // !MAR_ENGINE_MAIN_H