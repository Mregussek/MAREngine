/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"
#include "../LayerLogs.h"
#include "../../Editor/GUI/GUI.h"
#include "../../Editor/GUI/GUI_Graphics.h"
#include "../../Editor/Camera/Camera.h"


namespace mar {
	namespace layers {


		LayerGUI::LayerGUI(const char* name)
			: m_debugName(name)
		{ }

		void LayerGUI::initialize(editor::GUI* gui, editor::Camera* cam) {
			LAYER_TRACE("GUI_LAYER: {} going to initialize", m_debugName);

			m_gui = gui;
			m_camera = cam;

			m_gui->initialize("#version 330");

			m_camera->initialize();

			/* VIEWPORT SETUP */
			m_camera->setWindowSize(&m_gui->getViewportWidth(), &m_gui->getViewportHeight());

			/* MOUSE SETUP */
			const auto& win = window::Window::getInstance();
			m_camera->setMouseCall(&win.getMouseX(), &win.getMouseY());
			m_camera->setScrollCall(&win.getScrollX(), &win.getScrollY());

			/* SETUP RENDERING GUIZMO / LINE LOOPS */
			editor::GUI_Graphics::getInstance().initialize();

			LAYER_INFO("GUI_LAYER: {} initialized", m_debugName);
		}

		void LayerGUI::update() {
			LAYER_TRACE("GUI_LAYER: {} going to display frame", m_debugName);

			editor::GUI_Graphics::getInstance().passToDrawEntity(m_gui->getCurrentEntity(), m_gui->canDrawLines());

			m_camera->processInput();
			m_camera->updateData();

			m_gui->display();

			LAYER_INFO("GUI_LAYER: {} displayed frame", m_debugName);
		}

		void LayerGUI::closeLayer() {
			LAYER_TRACE("GUI_LAYER: {} going to close!", m_debugName);

			m_gui->shutdown();

			editor::GUI_Graphics::getInstance().close();

			LAYER_INFO("GUI_LAYER: {} closed!", m_debugName);
		}

		editor::GUI* LayerGUI::getGUIInstance() { 
			return m_gui; 
		}

		editor::Camera* LayerGUI::getCamera() {
			return m_camera; 
		}


} }