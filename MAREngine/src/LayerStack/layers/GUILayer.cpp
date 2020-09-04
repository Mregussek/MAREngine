/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"
#include "../LayerLogs.h"
#include "../../Editor/GUI/GUI.h"
#include "../../Editor/GUI/GUI_Graphics.h"
#include "../../Editor/Camera/Camera.h"
#include "../../Window/Input.h"


namespace mar {
	namespace layers {


		LayerGUI::LayerGUI(const char* name)
			: m_debugName(name),
			m_gui(nullptr),
			m_camera(nullptr)
		{ }

		void LayerGUI::initialize(editor::GUI* gui, editor::Camera* cam) {
			LAYER_TRACE("GUI_LAYER: {} going to initialize", m_debugName);

			m_gui = gui;
			m_camera = cam;

			m_gui->initialize("#version 330");

			m_camera->aspectRatio = m_gui->getViewportWidth() / m_gui->getViewportHeight();

			m_camera->initialize();
			m_camera->updateData();

			editor::GUI_Graphics::getInstance().initialize();

			LAYER_INFO("GUI_LAYER: {} initialized", m_debugName);
		}

		void LayerGUI::update() {
			LAYER_TRACE("GUI_LAYER: {} going to display frame", m_debugName);

			editor::GUI_Graphics::getInstance().passToDrawEntity(m_gui->getCurrentEntity(), m_gui->canDrawLines());

			static bool last_input_state = m_gui->isViewportInputEnabled();

			if (m_gui->isViewportInputEnabled()) {
				bool firstMouse = last_input_state != m_gui->isViewportInputEnabled() ? false : true;
				last_input_state = m_gui->isViewportInputEnabled();

				m_camera->processInput();
				//m_camera->ProcessMouseMovement(m_gui->getMouseViewportPosX(), m_gui->getMouseViewportPosY(), false, firstMouse);
				//m_camera->ProcessMouseScroll(window::Input::getScrollY());
			}

			m_camera->aspectRatio = m_gui->getViewportWidth() / m_gui->getViewportHeight();
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