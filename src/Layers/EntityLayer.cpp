/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "EntityLayer.h"
#include "../Core/Renderer/Renderer.h"


namespace mar {
	namespace layers {


		void EntityLayer::initialize(const std::vector<uint32_t>& layout) {
			m_renderer = new graphics::RendererOpenGL();

			m_renderer->initialize(layout, SHADER_DEFAULT);
		}

		void EntityLayer::prepareFrame() {
			if (storage::usegui) {
				m_framebuffer.bind();
				m_framebuffer.clear();
				m_framebuffer.unbind();

				MAR_CORE_TRACE("MESH_LAYER: cleared framebuffer");
				return;
			}
		}

		void EntityLayer::update() {
			if (storage::usegui) {
				m_framebuffer.bind();

				m_scene.update();
				//m_renderer->draw(&m_scene);

				m_framebuffer.unbind();

				MAR_CORE_TRACE("MESH_LAYER: Scene loaded to framebuffer");
			}
		}

		void EntityLayer::endFrame() {

		}

		void EntityLayer::closeLayer() {
			delete m_renderer;
		}


} }