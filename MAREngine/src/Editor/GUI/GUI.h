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
#include "../../Platform/OpenGL/FramebufferOpenGL.h"
#include "GUI_MainMenuBar.h"
#include "EntityPanels/GUI_EntityPanel.h"
#include "EntityPanels/GUI_EntityCollectionPanel.h"
#include "EntityPanels/GUI_SceneHierarchy.h"
#include "GUI_TextEditor.h"


namespace mar::ecs { class SceneManager; class Entity; class EntityCollection; /* forward declarations */ }
namespace mar::graphics { class RenderCamera; }
namespace mar::editor {


	class GUI {
	public:

		GUI() = default;

		void initialize(const char* glsl_version);
		void shutdown();

		void submit(ecs::SceneManager* scene);

		void bind() const;

		void display();

		// --- GET METHODS --- //
		float getViewportWidth() { return m_viewportFramebuffer.getSpecification().width; }
		float getViewportHeight() { return m_viewportFramebuffer.getSpecification().height; }
		float getMouseViewportPosX() { return m_mouseViewportX; }
		float getMouseViewportPosY() { return m_mouseViewportY; }
		bool isViewportInputEnabled() { return m_enableViewportInput; }
		platforms::FramebufferOpenGL& getFramebuffer();
		const ecs::Entity& getCurrentEntity() const;
		const ecs::EntityCollection& getCurrentCollection() const;
		bool canDrawLines() const;

	private:

		void prepareNewFrame();
		void updateFrame();
		void endFrame() const;

		void Editor_ViewPort();
		void Editor_Properties();

	private:

		ecs::SceneManager* m_sceneManager;

		platforms::FramebufferOpenGL m_viewportFramebuffer;
		float m_mouseViewportX{ 0.f };
		float m_mouseViewportY{ 0.f };
		bool m_enableViewportInput{ false };

		GUI_MainMenuBar m_mainMenuBar;
		GUI_EntityPanel m_entityPanel;
		GUI_EntityCollectionPanel m_collectionPanel;
		GUI_SceneHierarchy m_sceneHierarchyPanel;
		GUI_TextEditor m_textEditor;

		bool m_dockspaceOpen{ true };
		static const bool m_fullscreenPersisant{ true };

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_H