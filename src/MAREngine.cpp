/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"

#define USE_GUI

namespace mar {
    namespace engine {


		void MAREngine::initialize() {
#ifdef USE_GUI
			bool usegui = true;
#else
			bool usegui = false;
#endif

			m_camera.initialize(MAREngineDefaultSettings.width, MAREngineDefaultSettings.height);
			m_window.initialize(MAREngineDefaultSettings.height, MAREngineDefaultSettings.width, MAREngineDefaultSettings.name, &m_camera);
#ifdef USE_GUI
			m_gui.initialize(&m_window, MAREngineDefaultSettings.glsl_version, usegui);
#endif
			m_renderer.createRenderer(std::make_shared<graphics::RendererOpenGLFactory>());

			m_scene.initializeScene(graphics::SceneType::WITH_COLOURED_ELEMS);
			m_mesh.createMesh();
			m_renderer.loadScene(&m_mesh, &m_scene);

#ifdef USE_GUI
			m_gui.loadSceneParameters(&m_scene);
#else
			m_scene.initializeScene(graphics::SceneType::SECOND_MESH);
			m_secondMesh.createMesh();
			m_renderer.loadScene(&m_secondMesh, &m_scene);
#endif
			m_renderer.initialize(m_mesh.getLayout(), usegui);
		}

		void MAREngine::run() {
			while (m_window.shouldClose()) {
				m_camera.processInput(m_window.getWindow());
				m_camera.updateData();

#ifdef USE_GUI
				m_gui.prepareNewFrame();
				m_renderer.updateGUIData(&m_mesh, &m_gui.getGUIData());
#endif

				m_renderer.updateCameraData(&m_camera.getCameraData());
				m_renderer.draw(&m_mesh);

#ifdef USE_GUI
				m_gui.updateSceneInfo(&m_mesh, &m_renderer.getStatistics());
				m_gui.display();
#else
				m_renderer.draw(&m_secondMesh);
#endif
				m_window.swapBuffers();
			}
		}

		void MAREngine::shutdown() {
#ifdef USE_GUI
			m_gui.shutdown();
#endif
			m_renderer.closeRenderer();
			m_window.shutdown();
		}


} }