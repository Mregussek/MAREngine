/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"
#include "../../Debug/Log.h"
#include "../../Editor/GUI/GUI.h"
#include "../../Editor/Camera/Camera.h"


namespace mar {
	namespace layers {


		LayerGUI::LayerGUI(const char* name)
			: m_debugName(name)
		{ }

		void LayerGUI::initialize(editor::GUI* gui, editor::Camera* cam) {
			m_gui = gui;
			m_camera = cam;

			m_gui->initialize(settings::glsl_version);

			m_camera->initialize();

			/* VIEWPORT SETUP */
			m_camera->setWindowSize(&m_gui->getViewportWidth(), &m_gui->getViewportHeight());

			/* MOUSE SETUP */
			auto win = window::Window::getInstance();
			m_camera->setMouseCall(&win.getMouseX(), &win.getMouseY());
			m_camera->setScrollCall(&win.getScrollX(), &win.getScrollY());

			MAR_CORE_INFO("GUI_LAYER: initialized");
		}

		void LayerGUI::update() {
			m_camera->processInput();
			m_camera->updateData();

			m_gui->display();

			MAR_CORE_TRACE("GUI_LAYER: displaying frame");
		}

		void LayerGUI::closeLayer() {
			m_gui->shutdown();

			MAR_CORE_INFO("GUI_LAYER: closed!");
		}

		editor::GUI* LayerGUI::getGUIInstance() { 
			return m_gui; 
		}

		editor::Camera* LayerGUI::getCamera() {
			return m_camera; 
		}


} }