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
			graphics::Renderer m_renderer;
			graphics::Mesh m_mesh;
			graphics::Renderer m_cubemapRenderer;
			graphics::Mesh m_cubemapMesh;

			m_camera.initialize(MAREngineSettings::width, MAREngineSettings::height);
			m_window.initialize(MAREngineSettings::height, MAREngineSettings::width, MAREngineSettings::name, &m_camera);
			m_gui.initialize(&m_window, MAREngineSettings::glsl_version, usegui);

			m_renderer.createRenderer(factory, graphics::RendererType::DEFAULT);
			m_mesh.createMesh(factory);

			m_mesh.loadScene(&graphics::Scene(graphics::SceneType::WITH_COLOURED_ELEMS), graphics::MeshTextures::TEXTURES);
			m_renderer.initialize(m_mesh.getLayout());
			m_renderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());

			m_cubemapRenderer.createRenderer(factory, graphics::RendererType::CUBEMAP);
			m_cubemapMesh.createMesh(factory);

			m_cubemapMesh.loadScene(&graphics::Scene(graphics::SceneType::CUBEMAPS), graphics::MeshTextures::CUBEMAPS);
			m_cubemapRenderer.initialize(m_cubemapMesh.getLayout());
			m_cubemapRenderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());

			m_gui.setReferences(&m_mesh, &graphics::Renderer::getStatistics());

			while (m_window.shouldClose()) {
				m_camera.processInput(m_window.getWindow());
				m_camera.updateData();

				graphics::Renderer::getStatistics().resetStatistics();

				m_renderer.draw(&m_mesh);
				m_cubemapRenderer.draw(&m_cubemapMesh);
				
				m_gui.prepareNewFrame();
				m_gui.display();

				m_window.swapBuffers();
			}

			m_gui.shutdown();
			m_renderer.closeRenderer();
			m_window.shutdown();
		}


} }