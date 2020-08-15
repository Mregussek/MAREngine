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
// Scripting
#include "Core/scripting/PythonScript.h"
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

			// --- GET METHODS --- //

			static MAREngine* getEngine() { return &s_instance; }

			// -----------------------------------------
			// END-USER METHODS
			// -----------------------------------------

			// --- ENGINE MANAGEMENT --- //

			void setRestart() { m_shouldRestart = true; }
			void setNoRestart() { m_shouldRestart = false; }
			const bool shouldEngineRestart() { return m_shouldRestart; }
			void setLoadPath(std::string path) { m_pathLoad = path; }

			// --- WINDOW MANAGEMENT --- //

			void initWindow(const int& height, const int& width, const char* name);
			void closeWindow();

			void clearWindowScreen();
			void swapWindowBuffers();

			const bool shouldWindowClose();
			
			void updateBackground(editor::GUI* gui, ecs::Scene* scene);
			void updateBackground(ecs::Scene* scene);

			// --- LAYERS MANAGEMENT --- //

			editor::GUI createGUI();
			editor::Camera createGUICamera();

			layers::LayerStack createLayerStack();
			layers::EntityLayer* createEntityLayer();
			layers::LayerGUI* createEditorLayer();

			// --- SCENE MANAGEMENT --- //

			ecs::Scene* loadSceneFromFile();

			// --- RENDERING MANAGEMENT --- //

			void resetStatistics() { graphics::RendererEntity::clearStatistics(); }

		};


} }

#endif // !MAR_ENGINE_MAIN_H