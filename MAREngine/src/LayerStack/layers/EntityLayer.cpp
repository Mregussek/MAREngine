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


namespace mar {
	namespace layers {


		EntityLayer::EntityLayer(const char* debugname)
			: m_debugName(debugname)
		{}

		void EntityLayer::initialize(ecs::Scene* scene) {
			LAYER_TRACE("ENTITY_LAYER: {} going to initialize!", m_debugName);

			auto& ren = graphics::RenderPipeline::getInstance();
			ren = graphics::RenderPipeline();
			ren.initialize();

			m_renderer.initialize();

			ecs::SceneEvents::Instance().scene_manager = &m_sceneManager;
			m_sceneManager.setScene(scene);
			m_sceneManager.initialize();

			LAYER_INFO("ENTITY_LAYER: {} initialized!", m_debugName);
		}

		void EntityLayer::update() {
			LAYER_TRACE("ENTITY_LAYER: {} going to update", m_debugName);
			
			ecs::SceneEvents::Instance().scene_manager = &m_sceneManager;
			m_sceneManager.update();
			m_renderer.draw(graphics::RenderPipeline::getInstance());

			LAYER_INFO("ENTITY_LAYER: {} updated!", m_debugName);
		}

		void EntityLayer::closeLayer() {
			LAYER_TRACE("ENTITY_LAYER: {} going to close", m_debugName);

			graphics::RenderPipeline::getInstance().reset();

			m_renderer.close();
			m_sceneManager.shutdown();

			LAYER_INFO("ENTITY_LAYER: {} closed!", m_debugName);
		}

		ecs::SceneManager* EntityLayer::getSceneManager() {
			return &m_sceneManager; 
		}


} }