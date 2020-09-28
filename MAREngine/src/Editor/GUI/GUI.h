/**
 *           MAREngine - open source 3D game engine
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
#include "GUI_MainMenuBar.h"
#include "../../Platform/OpenGL/FrameBufferOpenGL.h"


namespace mar {
	namespace ecs { class SceneManager; class Entity; class EntityCollection; /* forward declarations */ }

	namespace editor {


		class GUI {
		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void submit(ecs::SceneManager* scene);

			void bind() {
				m_viewportFramebuffer.bind();
				m_viewportFramebuffer.clear();
			}

			void display();

			// --- GET METHODS --- //
			float getViewportWidth() { return m_viewportFramebuffer.getSpecification().width; }
			float getViewportHeight() { return m_viewportFramebuffer.getSpecification().height; }
			float getMouseViewportPosX() { return m_mouseViewportX; }
			float getMouseViewportPosY() { return m_mouseViewportY; }
			bool isViewportInputEnabled() { return m_enableViewportInput; }
			platforms::FrameBufferOpenGL& getFramebuffer() { return m_viewportFramebuffer; }
			ecs::Entity* getCurrentEntity();
			ecs::EntityCollection* getCurrentCollection();
			bool canDrawLines();

		private:
			// --- DISPLAY --- //
			void prepareNewFrame();
			void updateFrame();
			void endFrame();

			// --- EDITOR --- //
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
			// --- MainMenuBar
			GUI_MainMenuBar m_mainMenuBar;
			// --- Dockspace
			bool s_dockspaceOpen{ true };
			bool s_fullscreenPersisant{ true };
		};


} }

#endif // !MAR_ENGINE_EDITOR_GUI_H