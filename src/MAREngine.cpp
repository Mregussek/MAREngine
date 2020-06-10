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

			m_scene.initializeScene(graphics::SceneType::WITH_COLOURED_ELEMS);
			m_mesh.loadScene(&m_scene);
			m_gui.loadSceneParameters(&m_scene);

			m_renderer.initialize(m_mesh.getLayout(), usegui);
		}

		void MAREngine::run() {
			while (m_window.shouldClose()) {
				m_camera.processInput(m_window.getWindow());
				m_camera.updateData();
				m_gui.prepareNewFrame();

				m_renderer.bind();

				m_renderer.updateGUIData(&m_mesh, &m_gui.getGUIData());
				m_renderer.updateCameraData(&m_camera.getCameraData());
				m_renderer.updateLightData(&m_mesh);

				m_renderer.draw(&m_mesh);

				m_renderer.unbind();

				m_gui.updateSceneInfo(&m_mesh, &m_renderer.getStatistics());
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