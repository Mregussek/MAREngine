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


#include "EntityLayer.h"
#include "../LayerLogs.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneEvents.h"
#include "../../Core/graphics/Renderer/RenderPipeline.h"
#include "../../Core/graphics/Renderer/RenderEvents.h"
#include "../../Window/Window.h"


namespace mar::layers {


	EntityLayer::EntityLayer(const char* debugname) {
		p_debugName = debugname;
	}
	
	void EntityLayer::passSceneToManager(ecs::Scene* scene) {
		m_sceneManager.setScene(scene);

		LAYER_TRACE("ENTITY_LAYER: {} passed scene to scenemanager");
	}

	void EntityLayer::initialize() {
		LAYER_TRACE("ENTITY_LAYER: {} going to initialize!", m_debugName);

		ecs::SceneEvents::Instance().setSceneManager(m_sceneManager);
		graphics::RenderEvents::Instance().setRenderPipeline(m_renderPipeline);

		m_renderPipeline.initialize();
		m_renderer.initialize();

		m_sceneManager.initialize();

		LAYER_INFO("ENTITY_LAYER: {} initialized!", m_debugName);
	}

	void EntityLayer::update() {
		LAYER_TRACE("ENTITY_LAYER: {} going to update", m_debugName);
		
		m_renderPipeline.setCurrentPipeline();
		m_renderPipeline.getStatistics().resetStatistics();

		ecs::SceneEvents::Instance().setSceneManager(m_sceneManager);
		graphics::RenderEvents::Instance().setRenderPipeline(m_renderPipeline);

		m_sceneManager.update();
		
		const bool isWindowMinimized = window::Window::getSizeX() < 2 && window::Window::getSizeY() < 2;

		m_renderer.draw(m_renderPipeline);
		
		LAYER_INFO("ENTITY_LAYER: {} updated!", m_debugName);
	}

	void EntityLayer::closeLayer() {
		LAYER_TRACE("ENTITY_LAYER: {} going to close", m_debugName);

		m_renderPipeline.setCurrentPipeline();
		m_renderPipeline.reset();
		m_renderer.close();
		m_sceneManager.shutdown();

		LAYER_INFO("ENTITY_LAYER: {} closed!", m_debugName);
	}

	ecs::SceneManager* EntityLayer::getSceneManager() {
		return &m_sceneManager; 
	}


}
