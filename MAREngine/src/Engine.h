/**
 *				MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


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
#include "Core/graphics/Renderer/RendererBatch.h"
// Scripting
#include "Core/scripting/PythonScript.h"
// Window && Input
#include "Window/Input.h"
#include "Window/Window.h"


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

			void initWindow(int32_t height, int32_t width, const char* name);
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

			void resetStatistics() { graphics::RenderPipeline::getInstance().clearStatistics(); }

			// --- SCRIPTING MANAGEMENT --- //

			void initializeScripting() { scripting::PythonScript::appendCurrentPath(); }

		};


} }

#endif // !MAR_ENGINE_MAIN_H