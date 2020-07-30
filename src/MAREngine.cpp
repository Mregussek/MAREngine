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

			storage::usegui = true;

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			layers::LayerStack m_stack;

			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			m_framebuffer.initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initialize();
			if (storage::usegui)
				camera_layer->getCamera()->setWindowSize(&m_framebuffer.getSpecification().width, &m_framebuffer.getSpecification().height);
			else
				camera_layer->getCamera()->setWindowSize((const float*)&window::Window::getInstance().getWidth(), (const float*)&window::Window::getInstance().getHeight());
			camera_layer->mouseSetup();

			auto gui_layer = new layers::LayerGUI("Default GUI Layer");
			gui_layer->initialize();
			auto gui = gui_layer->getGUIInstance();
			gui->set(m_framebuffer);

			m_stack.pushLayer(camera_layer);
			m_stack.pushOverlay(gui_layer);
			
			auto entitylayer = new layers::EntityLayer("Entity Layer");
			auto scene = editor::Filesystem::openFile(m_pathLoad.c_str());
			entitylayer->initialize(scene);
			m_stack.pushLayer(entitylayer);
			gui->submit(entitylayer->getScene());

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();

				if (storage::usegui) {
					m_framebuffer.bind();
					m_framebuffer.clear();

					entitylayer->getRenderer()->submit(entitylayer->getScene());

					m_stack.update();

					graphics::RendererEntity::clearStatistics();
				}
				else {
					m_stack.update();
				}

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

			MAR_CORE_INFO("ENGINE: user has exited main loop!");

			m_framebuffer.close();
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