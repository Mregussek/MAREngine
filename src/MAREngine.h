/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


// Log && Filesystem
#include "Debug/Log.h"
#include "Editor/FileSystem/filesystem.h"
// Layers
#include "Layers/LayerStack.h"
#include "Layers/GUILayer.h"
#include "Layers/MeshLayer.h"
#include "Layers/CameraLayer.h"
// Rendering
#include "Core/Mesh/Mesh.h"
#include "Core/Scene/Scene.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RendererFactory.h"
#include "Core/Renderer/RendererOpenGLFactory.h"
// Window && Input
#include "Window/Input.h"
#include "Window/Window.h"
#include "Editor/GUI/GUI.h"
#include "Devices/SerialPortMonitor.h"


namespace mar {
	namespace engine {


		class MAREngine {
			static MAREngine s_instance;

			std::string m_pathLoad{ "resources/mar_files/load_default.marscene" };

			Ref<graphics::RendererFactory> m_factory{ graphics::RendererOpenGLFactory::createFactory() };
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
		};


} }

#endif // !MAR_ENGINE_MAIN_H