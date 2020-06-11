/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"


namespace mar {
    namespace engine {


		void MAREngine::initialize() {
			MAR_LOG_INIT();

			bool usegui = true;
			auto factory = std::make_shared<graphics::RendererOpenGLFactory>();

			m_camera.initialize(MAREngineDefaultSettings.width, MAREngineDefaultSettings.height);
			m_window.initialize(MAREngineDefaultSettings.height, MAREngineDefaultSettings.width, MAREngineDefaultSettings.name, &m_camera);
			m_gui.initialize(&m_window, MAREngineDefaultSettings.glsl_version, usegui);
			m_renderer.createRenderer(factory);
			m_mesh.createMesh(factory);

			m_mesh.loadScene(&graphics::Scene(graphics::SceneType::WITH_COLOURED_ELEMS));

			m_renderer.initialize(m_mesh.getLayout(), usegui);
			m_renderer.setReferences(&gui::GUI::getGUIData(), &m_camera.getCameraData());
			m_gui.setReferences(&m_mesh, &graphics::Renderer::getStatistics());
		}

		void MAREngine::run() {
			while (m_window.shouldClose()) {
				m_camera.processInput(m_window.getWindow());
				m_camera.updateData();
				m_gui.prepareNewFrame();

				m_renderer.draw(&m_mesh);
				
				m_gui.display();

				m_window.swapBuffers();
			}
		}

		void MAREngine::shutdown() {
			m_gui.shutdown();
			m_renderer.closeRenderer();
			m_window.shutdown();
		}


} }