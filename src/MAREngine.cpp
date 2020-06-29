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


		void MAREngine::run() {
			MAR_LOG_INIT();

			layers::LayerStack m_stack;

			auto m_factory = graphics::RendererOpenGLFactory::createFactory();
			bool m_usegui = true;

			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name);
			window::Input::initialize(m_window.getWindow());
			window::Input::enableInput();
			
			gui::GUI m_gui;
			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, m_usegui);

			auto m_framebuffer = m_factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			layers::CameraLayer* camera_layer = new layers::CameraLayer("Default Camera Layer");
			camera_layer->initializeLayer(new graphics::Camera());
			camera_layer->set(&m_framebuffer->getSpecification().width, &m_framebuffer->getSpecification().height);
			camera_layer->set(&m_window);

			layers::GUILayer* gui_layer = new layers::GUILayer("Default GUI Layer");
			gui_layer->initializeLayer(&m_gui);
			gui_layer->set(&graphics::Renderer::getStatistics(), m_framebuffer);

			m_stack.pushOverlay(gui_layer);
			m_stack.pushLayer(camera_layer);

			std::vector<std::pair<graphics::SceneType, graphics::MeshType>> types = {
				{DEFAULT_SCENE, NORMAL_MESH_TYPE},
				//{CUBEMAPS_SCENE, CUBEMAPS_MESH_TYPE},
				{OBJECTS_SCENE, OBJECTS_MESH_TYPE}
			};

			for (unsigned int i = 0; i < types.size(); i++) {
				std::string name = "Mesh Layer " + std::to_string(i);
				layers::MeshLayer* layer = new layers::MeshLayer(name.c_str());
				layer->initializeLayer(new graphics::Renderer(), new graphics::Mesh());
				layer->create(m_factory, m_usegui);
				layer->scene(types[i].first, types[i].second);
				layer->set(&gui::GUI::getGUIData(), &camera_layer->getCameraData());
				layer->set(m_framebuffer);
				m_stack.pushLayer(layer);
				gui_layer->submit(layer->getMesh());
			}

			while (m_window.shouldClose()) {
				m_window.clearScreen();
				
				graphics::Renderer::getStatistics().resetStatistics();

				m_stack.update();

				m_window.swapBuffers();
			}

			m_framebuffer->close();

			m_stack.close();
			m_window.shutdown();
		}


} }