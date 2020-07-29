/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUILayer.h"
#include "../../Debug/Log.h"
#include "../../Editor/GUI/GUI.h"


namespace mar {
	namespace layers {


		LayerGUI::LayerGUI(const char* name)
			: m_debugName(name),
			m_gui(nullptr)
		{ }

		void LayerGUI::initialize() {
			m_gui = new editor::GUI();
			m_gui->initialize(settings::glsl_version);

			MAR_CORE_INFO("GUI_LAYER: initialized");
		}

		void LayerGUI::update() {
			m_gui->display();

			MAR_CORE_TRACE("GUI_LAYER: displaying frame");
		}

		void LayerGUI::closeLayer() {
			m_gui->shutdown();
			delete m_gui;

			MAR_CORE_INFO("GUI_LAYER: closed!");
		}

		editor::GUI* LayerGUI::getGUIInstance() { 
			return m_gui; 
		}


} }