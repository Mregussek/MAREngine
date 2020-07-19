/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"
#include "../Debug/Log.h"


namespace mar {
	namespace layers {


		void GUILayer::initialize() {
			m_gui = new editor::GUI();
			m_gui->initialize(settings::glsl_version);

			MAR_CORE_INFO("GUI_LAYER: initialized");
		}

		void GUILayer::set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuf) {
			m_gui->setFrameBuffer(framebuf);
			MAR_CORE_TRACE("GUI_LAYER: Framebuffer is set");
		}

		void GUILayer::submit(graphics::Mesh* mesh) {
			m_gui->submitMesh(mesh);

			MAR_CORE_INFO("GUI_LAYER: submitted mesh");
		}

		void GUILayer::prepareFrame() {
			m_gui->prepareNewFrame();

			MAR_CORE_TRACE("GUI_LAYER: preparing frame");
		}

		void GUILayer::update() {
			m_gui->display();

			MAR_CORE_TRACE("GUI_LAYER: displaying frame");
		}

		void GUILayer::closeLayer() {
			m_gui->shutdown();

			delete m_gui;

			MAR_CORE_INFO("GUI_LAYER: closed!");
		}


} }