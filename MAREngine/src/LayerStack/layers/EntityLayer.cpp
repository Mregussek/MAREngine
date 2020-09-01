/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EntityLayer.h"
#include "../LayerLogs.h"


namespace mar {
	namespace layers {


		EntityLayer::EntityLayer(const char* debugname)
			: m_debugName(debugname)
		{}

		void EntityLayer::initialize(ecs::Scene* scene) {
			LAYER_TRACE("ENTITY_LAYER: {} going to initialize!", m_debugName);

			m_renderer.initialize();

			m_sceneManager.setScene(scene);
			m_sceneManager.initialize();

			LAYER_INFO("ENTITY_LAYER: {} initialized!", m_debugName);
		}

		void EntityLayer::update() {
			LAYER_TRACE("ENTITY_LAYER: {} going to update", m_debugName);

			m_sceneManager.update();
			m_renderer.draw(graphics::RenderPipeline::getInstance());

			LAYER_INFO("ENTITY_LAYER: {} updated!", m_debugName);
		}

		void EntityLayer::closeLayer() {
			LAYER_TRACE("ENTITY_LAYER: {} going to close", m_debugName);

			m_renderer.close();
			m_sceneManager.shutdown();

			LAYER_INFO("ENTITY_LAYER: {} closed!", m_debugName);
		}

		graphics::RendererBatch* EntityLayer::getRenderer() {
			return &m_renderer;
		}

		ecs::SceneManager* EntityLayer::getSceneManager() {
			return &m_sceneManager; 
		}


} }