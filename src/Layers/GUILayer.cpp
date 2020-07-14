/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"


namespace mar {
	namespace layers {


		void GUILayer::initialize() {
			m_gui = new editor::GUI();
			m_gui->initialize(settings::glsl_version);
		}

		void GUILayer::set(const Ref<graphics::FrameBuffer>& framebuf) {
			if (framebuf != nullptr)
				m_gui->setFrameBuffer(framebuf);
		}

		void GUILayer::submit(graphics::Mesh* mesh) {
			m_gui->submitMesh(mesh);
		}

		void GUILayer::update() {
			m_gui->prepareNewFrame();
			m_gui->display();
		}

		void GUILayer::closeLayer() {
			m_gui->shutdown();

			delete m_gui;
		}


} }