/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


// Log && Filesystem
#include "Debug/Log.h"
#include "Editor/GUI/GUI.h"
#include "Editor/Filesystem/EditorFilesystem.h"
#include "Editor/Camera/Camera.h"
// Layers
#include "LayerStack/LayerStack.h"
#include "LayerStack/layers/GUILayer.h"
#include "LayerStack/layers/EntityLayer.h"
// Rendering
#include "Core/ecs/Scene.h"
#include "Core/graphics/Renderer/RendererEntity.h"
// Window && Input
#include "Window/Input.h"
#include "Window/Window.h"
#include "Devices/SerialPortMonitor.h"


namespace mar {
	namespace engine {


		class MAREngine {
			static MAREngine s_instance;

			std::string m_pathLoad{ "resources/mar_files/empty.marscene" };

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