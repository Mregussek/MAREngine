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

			storage::factory = graphics::RendererOpenGLFactory::createFactory();
			storage::usegui = true;

			window::Window::getInstance().initialize(settings::height, settings::width, settings::name);
			window::Input::initialize(window::Window::getInstance().getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			auto m_framebuffer = storage::factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initialize();
			if (storage::usegui)
				camera_layer->set(m_framebuffer->getSpecification().width, m_framebuffer->getSpecification().height);
			else
				camera_layer->set(window::Window::getInstance().getWidth(), window::Window::getInstance().getHeight());
			camera_layer->mouseSetup();

			auto gui_layer = new layers::GUILayer("Default GUI Layer");
			gui_layer->initialize();
			gui_layer->set(m_framebuffer);

			layers::LayerStack m_stack;
			m_stack.pushOverlay(gui_layer);
			m_stack.pushLayer(camera_layer);

			editor::filesystem::loadSceneFromFile(m_pathLoad);
			if (auto loaded = editor::filesystem::assignLoadedLayers(m_framebuffer)) 
				for (unsigned int i = 0; i < loaded->size(); i++) {
					gui_layer->submit(loaded->at(i)->getMesh());
					m_stack.pushLayer(loaded->at(i));
				}
			
			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (window::Window::getInstance().shouldClose() && !m_shouldRestart) 
			{
				window::Window::getInstance().clearScreen();
				graphics::Renderer::getStatistics().resetStatistics();
				
				m_stack.update();

				window::Window::getInstance().swapBuffers();
			}
			// --------------- RENDER LOOP -------------------- //
			//////////////////////////////////////////////////////

			m_framebuffer->close();
			m_stack.close();
		}

		void MAREngine::exit() {
		start_again:
			if (m_shouldRestart) {
				m_shouldRestart = false;
				run();
				goto start_again;
			}

			window::Window::getInstance().shutdown();
		}


} }