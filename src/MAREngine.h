/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


// Log && Filesystem
#include "Debug/Log.h"
#include "Editor/FileSystem/filesystem.h"
#include "Editor/GUI/GUI.h"
// Layers
#include "LayerStack/LayerStack.h"
#include "LayerStack/layers/GUILayer.h"
#include "LayerStack/layers/MeshLayer.h"
#include "LayerStack/layers/CameraLayer.h"
#include "LayerStack/layers/EntityLayer.h"
// Rendering
#include "Core/Light/Light.h"
#include "Core/Mesh/Mesh.h"
#include "Core/Scene/Scene.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/RendererOpenGL.h"
#include "Core/Renderer/RendererEntity.h"
// Window && Input
#include "Window/Input.h"
#include "Window/Window.h"
#include "Devices/SerialPortMonitor.h"


namespace mar {
	namespace engine {


		class MAREngine {
			static MAREngine s_instance;

			std::string m_pathLoad{ "resources/mar_files/another_default.marscene" };

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