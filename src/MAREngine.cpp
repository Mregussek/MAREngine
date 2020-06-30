/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"


namespace mar {
    namespace engine {


		const char MAREngineSettings::name[10] = "MAREngine";
		const int MAREngineSettings::width{ 1500 };
		const int MAREngineSettings::height{ 900 };
		const char MAREngineSettings::portName[10] = "\\\\.\\COM7";
		const char* MAREngineSettings::glsl_version = "#version 460";

		MAREngine MAREngine::s_instance;


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			m_usegui = true;

			filesystem::Storage::getInstance()->setup(m_factory, m_usegui);

			m_window = window::Window();

			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name);
			window::Input::initialize(m_window.getWindow());
			window::Input::enableInput();
		}

		void MAREngine::run() {
			gui::GUI m_gui;
			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, m_usegui);

			auto m_framebuffer = m_factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			auto camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initializeLayer(new graphics::Camera());
			if(m_usegui) camera_layer->set(&m_framebuffer->getSpecification().width, &m_framebuffer->getSpecification().height);
			else camera_layer->set((const float*)&MAREngineSettings::width, (const float*)&MAREngineSettings::height);
			camera_layer->set(&m_window);

			auto gui_layer = new layers::GUILayer("Default GUI Layer");
			gui_layer->initializeLayer(&m_gui);
			gui_layer->set(&graphics::Renderer::getStatistics(), m_framebuffer);

			layers::LayerStack m_stack;
			m_stack.pushOverlay(gui_layer);
			m_stack.pushLayer(camera_layer);

			auto vec = filesystem::fnc::loadSceneFromFile(m_pathLoad);

			for(auto& layer : vec) {
				layer->set(&gui::GUI::getGUIData(), &camera_layer->getCameraData());
				layer->set(m_framebuffer);
				m_stack.pushLayer(layer);
				gui_layer->submit(layer->getMesh());
			}

			//////////////////////////////////////////////////////
			// --------------- RENDER LOOP -------------------- //
			while (m_window.shouldClose() && !m_shouldRestart) 
			{
				m_window.clearScreen();
				
				graphics::Renderer::getStatistics().resetStatistics();

				m_stack.update();

				m_window.swapBuffers();
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

			m_window.shutdown();
		}

} }