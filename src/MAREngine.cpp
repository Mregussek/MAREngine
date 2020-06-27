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

			graphics::Camera m_camera;
			window::Window m_window;
			layers::LayerStack m_stack;

			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name, &m_camera);
			m_camera.initialize(MAREngineSettings::width, MAREngineSettings::height);
			m_camera.setReference(m_window.getWindow());

			auto m_factory = graphics::RendererOpenGLFactory::createFactory();
			bool m_usegui = true;

			gui::GUI m_gui;
			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, m_usegui);

			auto m_framebuffer = m_factory->createFrameBuffer();
			m_framebuffer->initialize(graphics::FrameBufferSpecification(800.f, 600.f));

			graphics::Renderer normal_renderer;	 graphics::Mesh normal_mesh;
			graphics::Renderer cubemap_renderer; graphics::Mesh cubemap_mesh;
			graphics::Renderer objects_renderer; graphics::Mesh objects_mesh;
			
			layers::MeshLayer normalmesh_layer("Normal Mesh Layer");
			layers::MeshLayer cubemapmesh_layer("CubeMap Mesh Layer");
			layers::MeshLayer objectsmesh_layer("Objects Mesh Layer");

			m_stack.pushLayer(&normalmesh_layer);
			m_stack.pushLayer(&cubemapmesh_layer);
			m_stack.pushLayer(&objectsmesh_layer);

			{
				normalmesh_layer.initializeLayer(&normal_renderer, &normal_mesh);
				normalmesh_layer.create(m_factory, m_usegui);
				normalmesh_layer.scene(DEFAULT_SCENE, NORMAL_MESH_TYPE);
			}
			{
				cubemapmesh_layer.initializeLayer(&cubemap_renderer, &cubemap_mesh);
				cubemapmesh_layer.create(m_factory, m_usegui);
				cubemapmesh_layer.scene(CUBEMAPS_SCENE, CUBEMAPS_MESH_TYPE);
			}
			{
				objectsmesh_layer.initializeLayer(&objects_renderer, &objects_mesh);
				objectsmesh_layer.create(m_factory, m_usegui);
				objectsmesh_layer.scene(OBJECTS_SCENE, OBJECTS_MESH_TYPE);
			}

			normalmesh_layer.set(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			normalmesh_layer.set(m_framebuffer);
			cubemapmesh_layer.set(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			cubemapmesh_layer.set(m_framebuffer);
			objectsmesh_layer.set(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			objectsmesh_layer.set(m_framebuffer);

			layers::GUILayer gui_layer("Default GUI Layer");
			m_stack.pushOverlay(&gui_layer);
			gui_layer.initializeLayer(&m_gui);
			gui_layer.set(&graphics::Renderer::getStatistics(), m_framebuffer);
			gui_layer.submit(normalmesh_layer.getMesh());
			gui_layer.submit(cubemapmesh_layer.getMesh());
			gui_layer.submit(objectsmesh_layer.getMesh());

			while (m_window.shouldClose()) {
				m_window.clearScreen();
				m_camera.processInput();
				m_camera.updateData();
				
				graphics::Renderer::getStatistics().resetStatistics();
				m_camera.setWindowSize(m_framebuffer->getSpecification().width, m_framebuffer->getSpecification().height);

				m_stack.update();

				m_window.swapBuffers();
			}

			m_framebuffer->close();

			m_stack.close();

			m_window.shutdown();
		}


} }