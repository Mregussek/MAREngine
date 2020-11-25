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


#include "../mar.h"
#include "../Platform/OpenGL/FramebufferOpenGL.h"
#include "GUIPanels/GUI_MainMenuBar.h"
#include "GUIPanels/EntityPanels/GUI_EntityPanel.h"
#include "GUIPanels/EntityPanels/GUI_EntityCollectionPanel.h"
#include "GUIPanels/EntityPanels/GUI_SceneHierarchy.h"
#include "GUIPanels/GUI_TextEditor.h"


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

		float getViewportAspectRatio() const;
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

		void updateViewportAspectRatio();

	private:

		platforms::FramebufferOpenGL m_viewportFramebuffer;

		GUI_MainMenuBar m_mainMenuBar;
		GUI_EntityPanel m_entityPanel;
		GUI_EntityCollectionPanel m_collectionPanel;
		GUI_SceneHierarchy m_sceneHierarchyPanel;
		GUI_TextEditor m_textEditor;

		ecs::SceneManager* m_sceneManager;

		float m_viewportAspectRatio{ 1.33f };

		bool m_dockspaceOpen{ true };
		static const bool m_fullscreenPersisant{ true };

	};


}


#endif // !MAR_ENGINE_EDITOR_GUI_H