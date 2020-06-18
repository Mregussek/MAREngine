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

			bool usegui = true;

			auto factory = std::make_shared<graphics::RendererOpenGLFactory>();
			graphics::Renderer loc_renderer;
			graphics::Mesh loc_mesh;
			graphics::Renderer loc_cubemapRenderer;
			graphics::Mesh loc_cubemapMesh;
			graphics::Renderer loc_objectRenderer;
			graphics::Mesh loc_objectMesh;

			layers::LayerStack loc_stack;
			layers::GUILayer loc_guilayer("Default GUI Layer");

			m_camera.initialize(MAREngineSettings::width, MAREngineSettings::height);
			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name, &m_camera);

			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, usegui);
			graphics::FrameBufferSpecification spec;
			spec.width = 800.f;
			spec.height = 600.f;
			auto loc_framebuffer = factory->createFrameBuffer();
			loc_framebuffer->initialize(spec);

			{
				loc_renderer.createRenderer(factory, usegui);
				loc_mesh.createMesh(factory);

				loc_mesh.loadScene(&graphics::Scene(DEFAULT_SCENE), NORMAL_MESH_TYPE);
				loc_renderer.initialize(loc_mesh.getLayout());
			}
			{
				loc_cubemapRenderer.createRenderer(factory, usegui);
				loc_cubemapMesh.createMesh(factory);

				loc_cubemapMesh.loadScene(&graphics::Scene(CUBEMAPS_SCENE), CUBEMAPS_MESH_TYPE);
				loc_cubemapRenderer.initialize(loc_cubemapMesh.getLayout(), graphics::ShaderType::CUBEMAP);
			}
			{
				loc_objectRenderer.createRenderer(factory, usegui);
				loc_objectMesh.createMesh(factory);

				loc_objectMesh.loadScene(&graphics::Scene(OBJECTS_SCENE), OBJECTS_MESH_TYPE);
				loc_objectRenderer.initialize(loc_objectMesh.getLayout());
			}
			
			loc_renderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			loc_cubemapRenderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			loc_objectRenderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());

			m_gui.setReferences(&graphics::Renderer::getStatistics());
			m_gui.setFrameBuffer(loc_framebuffer);

			m_gui.submitMesh(&loc_mesh);
			m_gui.submitMesh(&loc_cubemapMesh);
			m_gui.submitMesh(&loc_objectMesh);

			m_camera.setReference(m_window.getWindow());

			while (m_window.shouldClose()) {
				m_window.clearScreen();
				m_camera.processInput();
				m_camera.updateData();
				
				m_camera.setWindowSize(spec.width, spec.height);
				loc_framebuffer->bind();
				loc_framebuffer->clear();

				{
					graphics::Renderer::getStatistics().resetStatistics();

					loc_renderer.draw(&loc_mesh);
					loc_cubemapRenderer.draw(&loc_cubemapMesh);
					loc_objectRenderer.draw(&loc_objectMesh);
				}

				loc_framebuffer->unbind();

				{
					m_gui.prepareNewFrame();
					m_gui.display();
				}

				m_window.swapBuffers();
			}

			loc_framebuffer->close();
			m_gui.shutdown();

			loc_renderer.closeRenderer();
			loc_cubemapRenderer.closeRenderer();
			loc_objectRenderer.closeRenderer();
			m_window.shutdown();
		}


} }