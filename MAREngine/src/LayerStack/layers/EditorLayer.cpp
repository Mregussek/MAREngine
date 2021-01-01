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
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


	EditorLayer::EditorLayer(const char* name) {
		p_debugName = name;
	}

	void EditorLayer::initialize() {
		LAYER_TRACE("EDITOR_LAYER: {} going to initialize...", p_debugName);

		m_allEvents.entityEvents.Instance = &m_allEvents.entityEvents;

		m_editorManager.pushPanel(&m_allWidgets.textEditor);
		m_editorManager.pushPanel(&m_allWidgets.viewport);
		m_editorManager.pushPanel(&m_allWidgets.general);
		m_editorManager.pushPanel(&m_allWidgets.theme);

		// push filesystem panels
		m_editorManager.pushPanel(&m_allWidgets.filesystem);
		m_editorManager.pushPanel(&m_allWidgets.sceneFilesystem);
		m_editorManager.pushPanel(&m_allWidgets.entityFilesystem);

		m_editorManager.pushPanel(&m_allWidgets.sceneHierarchy);
		m_editorManager.pushPanel(&m_allWidgets.entity);

		m_editorManager.pushPanel(&m_allWidgets.statistics);
		m_editorManager.pushPanel(&m_allWidgets.mainMenuBar);
		m_editorManager.pushPanel(&m_allWidgets.sceneProperties);

		m_editorManager.create();
	}

	void EditorLayer::update() {
		LAYER_TRACE("EDITOR_LAYER: {} going to display frame...", p_debugName);

		m_editorManager.update();
	}

	void EditorLayer::close() {
		LAYER_TRACE("EDITOR_LAYER: {} going to close...", p_debugName);

		m_editorManager.destroy();
	}

	void EditorLayer::renderToViewport() {
		LAYER_TRACE("EDITOR_LAYER: {} starting rendering to viewport in Viewport Panel", p_debugName);

		m_allWidgets.viewport.bind(SceneManager::Instance->getScene()->getBackground());
	}


}
