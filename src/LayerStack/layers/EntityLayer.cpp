/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EntityLayer.h"


namespace mar {
	namespace layers {


		EntityLayer::EntityLayer(const char* debugname)
			: m_debugName(debugname)
		{}

		void EntityLayer::initialize(ecs::Scene* scene) {
			m_renderer.initialize();
		
			if (!scene)
				scene = new ecs::Scene("EmptyScene");

			m_sceneManager.setScene(scene);
		}

		void EntityLayer::update() {
			m_sceneManager.update();
			m_renderer.update(&m_sceneManager);
		}

		void EntityLayer::closeLayer() {
			m_renderer.close();
			m_sceneManager.shutdown();
		}

		graphics::RendererEntity* EntityLayer::getRenderer() {
			return &m_renderer;
		}

		ecs::SceneManager* EntityLayer::getSceneManager() {
			return &m_sceneManager; 
		}


} }