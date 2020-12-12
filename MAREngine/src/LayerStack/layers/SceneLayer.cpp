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
#include "../../Core/ecs/SceneEvents.h"


namespace mar::layers {


	SceneLayer::SceneLayer(const char* debugname) {
		p_debugName = debugname;
	}
	
	void SceneLayer::passSceneToManager(ecs::Scene* scene) {
		LAYER_TRACE("ENTITY_LAYER: passing to Scene to SceneManager!");

		m_sceneManager.setScene(scene);
	}

	void SceneLayer::initialize() {
		LAYER_TRACE("ENTITY_LAYER: {} going to initialize!", m_debugName);

		ecs::SceneEvents::Instance().setSceneManager(m_sceneManager);

		m_sceneManager.initialize();
	}

	void SceneLayer::close() {
		LAYER_TRACE("ENTITY_LAYER: {} going to close", m_debugName);

		m_sceneManager.shutdown();
	}

	void SceneLayer::update() {
		LAYER_TRACE("ENTITY_LAYER: {} going to update", m_debugName);
		
		ecs::SceneEvents::Instance().setSceneManager(m_sceneManager);
		m_sceneManager.update();
	}

	ecs::SceneManager* SceneLayer::getSceneManager() {
		return &m_sceneManager; 
	}


}
