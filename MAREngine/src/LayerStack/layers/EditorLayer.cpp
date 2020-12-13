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


#include "EditorLayer.h"
#include "../LayerLogs.h"


namespace mar::layers {


	EditorLayer::EditorLayer(const char* name) {
		p_debugName = name;
	}

	void EditorLayer::initialize() {
		LAYER_TRACE("GUI_LAYER: {} going to initialize", p_debugName);

		m_gui.initialize("#version 450");		

		//m_guiGraphics.initialize();

		LAYER_INFO("GUI_LAYER: {} initialized", p_debugName);
	}

	void EditorLayer::update() {
		LAYER_TRACE("GUI_LAYER: {} going to display frame", p_debugName);
		
		/*
		if (m_gui->canDrawLines()) {
			const auto& currentEntity = m_gui->getCurrentEntity();
			const bool userHasSelectedEntity = &currentEntity != nullptr;

			const auto& currentCollection = m_gui->getCurrentCollection();
			const bool userHasSelectedCollection = &currentCollection != nullptr;

			if (userHasSelectedCollection && !userHasSelectedEntity) {
				m_guiGraphics.passToDrawCollection(currentCollection);
			}
			else if (userHasSelectedEntity) {
				m_guiGraphics.passToDrawEntity(currentEntity);
			}
		}
		*/
		
		m_gui.display();

		LAYER_INFO("GUI_LAYER: {} displayed frame", p_debugName);
	}

	void EditorLayer::close() {
		LAYER_TRACE("GUI_LAYER: {} going to close!", p_debugName);

		m_gui.shutdown();

		//m_guiGraphics.close();

		LAYER_INFO("GUI_LAYER: {} closed!", p_debugName);
	}

	void EditorLayer::renderToViewport() {
		m_gui.renderToViewport();
	}

	void EditorLayer::submit(ecs::SceneManager* manager) {
		m_gui.submit(manager);
	}


}