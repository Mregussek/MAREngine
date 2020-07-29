/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EntityLayer.h"
#include "../../Core/Renderer/RendererEntity.h"


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
		
			m_scene = scene;
		}

		void EntityLayer::update() {
			m_renderer->submit(m_scene);
			m_renderer->update();
		}

		void EntityLayer::closeLayer() {
			m_renderer->close();

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