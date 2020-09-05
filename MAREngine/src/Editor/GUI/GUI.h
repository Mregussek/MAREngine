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


#ifndef MAR_ENGINE_EDITOR_GUI_H
#define MAR_ENGINE_EDITOR_GUI_H


#include "../../mar.h"
#include "../EditorLogging.h"

#include "../../Window/Window.h"

#include "../../Platform/OpenGL/FrameBufferOpenGL.h"
#include "../../Core/graphics/Renderer/RendererBatch.h"

#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneManager.h"

#include "GUI_EntityPanel.h"
#include "GUI_EntityCollectionPanel.h"
#include "GUI_Filesystem.h"
#include "GUI_Info.h"
#include "GUI_Theme.h"


namespace mar {
	namespace editor {


		class GUI {
		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void submit(ecs::SceneManager* scene);

			void display();

			// --- GET METHODS --- //
			float getViewportWidth() { return m_viewportFramebuffer.getSpecification().width; }
			float getViewportHeight() { return m_viewportFramebuffer.getSpecification().height; }
			float getMouseViewportPosX() { return m_mouseViewportX; }
			float getMouseViewportPosY() { return m_mouseViewportY; }
			bool isViewportInputEnabled() { return m_enableViewportInput; }
			platforms::FrameBufferOpenGL& getFramebuffer() { return m_viewportFramebuffer; }
			ecs::Entity* getCurrentEntity();
			bool canDrawLines() { return m_sceneManager->isEditorMode() && m_sceneManager->useEditorCamera; }

		private:
			// --- DISPLAY --- //
			void prepareNewFrame();
			void updateFrame();
			void endFrame();

			// --- EDITOR --- //
			void Editor_MainMenuBar();
			void Editor_ViewPort();
			void Editor_Properties();

		private:
			// --- Storage for scenes
			ecs::SceneManager* m_sceneManager;
			// --- Viewport
			platforms::FrameBufferOpenGL m_viewportFramebuffer;
			float m_mouseViewportX{ 0.f };
			float m_mouseViewportY{ 0.f };
			bool m_enableViewportInput{ false };
			// --- Dockspace
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			// --- Should Window Be Opened ?
			bool m_newSceneWindow{ false };
			bool m_saveSceneWindow{ false };
			bool m_loadSceneWindow{ false };
			bool m_loadOBJfileWindow{ false };
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };
		};


} }

#endif // !MAR_ENGINE_GUI_H