/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"


namespace mar {
    namespace engine {


		MAREngine MAREngine::s_instance;


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			layers::LayerStack m_stack;

			auto gui_layer = new layers::LayerGUI("Default GUI Layer");
			gui_layer->initialize();

			m_stack.pushOverlay(gui_layer);
			
			auto entitylayer = new layers::EntityLayer("Entity Layer");
			auto scene = editor::Filesystem::openFile(m_pathLoad.c_str());
			entitylayer->initialize(scene);
			gui_layer->getGUIInstance()->submit(scene);
			m_stack.pushLayer(entitylayer);

			scene->useEditorCamera = true;
			auto& framebuffer = gui_layer->getGUIInstance()->getFramebuffer();

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();

				framebuffer.bind();
				framebuffer.clear();

				m_stack.update();

				graphics::RendererEntity::clearStatistics();

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

			MAR_CORE_INFO("ENGINE: user has exited main loop!");

			m_stack.close();
		}

		void MAREngine::exit() {
		start_again:
			if (m_shouldRestart) {
				m_shouldRestart = false;
				
				MAR_CORE_INFO("ENGINE: restarting...");

				run();
				goto start_again;
			}

			window::Window::getInstance().shutdown();
		}


} }