/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"


namespace mar {
	namespace layers {


		void GUILayer::initializeLayer(gui::GUI* gui) {
			m_gui = gui;
		}

		void GUILayer::set(const graphics::RendererStatistics* stats, const Ref<graphics::FrameBuffer>& framebuf) {
			m_gui->setReferences(stats);

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
		}


} }