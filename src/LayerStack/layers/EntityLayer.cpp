/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EntityLayer.h"
#include "../../Core/graphics/Renderer/RendererEntity.h"


namespace mar {
	namespace layers {


		EntityLayer::EntityLayer(const char* debugname)
			: m_debugName(debugname),
			m_renderer(nullptr),
			m_scene(nullptr)
		{}

		void EntityLayer::initialize(ecs::Scene* scene) {
			m_renderer = new graphics::RendererEntity();
			m_renderer->initialize();
		
			if (scene)
				m_scene = scene;
			else
				m_scene = new ecs::Scene("EmptyScene");

			m_scene->initialize();
		}

		void EntityLayer::update() {
			m_scene->update();
			m_renderer->update(m_scene);
		}

		void EntityLayer::closeLayer() {
			m_renderer->close();
			m_scene->shutdown();

			delete m_scene;
			delete m_renderer;
		}

		graphics::RendererEntity* EntityLayer::getRenderer() {
			return m_renderer;
		}

		ecs::Scene* EntityLayer::getScene() {
			return m_scene; 
		}


} }