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


#include "SceneLayer.h"
#include "../LayerLogs.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


	SceneLayer::SceneLayer(const char* debugname) {
		p_debugName = debugname;
	}
	
	void SceneLayer::passSceneToManager(Scene* scene) {
		LAYER_TRACE("SCENE_LAYER: passing to Scene to SceneManager in {}!", p_debugName);

		m_sceneManager.setScene(scene);
	}

	void SceneLayer::initialize() {
		LAYER_TRACE("SCENE_LAYER: {} going to initialize...", p_debugName);

		m_eventsComponentEntity.Instance = &m_eventsComponentEntity;
		m_sceneManager.Instance = &m_sceneManager;

		m_sceneManager.initialize();
	}

	void SceneLayer::update() {
		m_sceneManager.update();
	}

	void SceneLayer::close() {
		LAYER_TRACE("SCENE_LAYER: {} going to close...", p_debugName);

		m_sceneManager.shutdown();
	}


}
