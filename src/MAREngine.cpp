/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "MAREngine.h"

namespace mar {
	void MAREngine::initialize() {
		// --- INITIALIZE MARENGINE MEMBERS --- //
		m_camera.initialize(MAREngineDefaultSettings.width, MAREngineDefaultSettings.height);
		m_window.initialize(MAREngineDefaultSettings.height, MAREngineDefaultSettings.width, MAREngineDefaultSettings.name, &m_camera);
		m_gui.initialize(&m_window, MAREngineDefaultSettings.glsl_version);
		m_scene.initializeScene(SceneType::WITH_COLOURED_ELEMS);
		m_renderer.createRenderer(std::make_shared<RendererOpenGLFactory>());

		// --- PUSH SCENE TO RENDERER AND SET GUI --- //
		m_renderer.loadScene(&m_scene);
		m_gui.loadSceneParameters(&m_scene);

		// --- INITIALIZE RENDERER WITH SCENE AND PREPARE FOR RENDERING --- //
		m_gui.connectToRenderer(&m_renderer);
		m_renderer.initialize();
		m_renderer.unbind();
	}

	void MAREngine::run() {
		while (m_window.shouldClose()) {
			// --- Processing Input --- //
			m_camera.processInput(m_window.getWindow());
			m_camera.updateData();
			m_gui.prepareNewFrame();

			// --- Renderer Setup before drawing --- //
			m_renderer.updateGUIData(&m_gui.getGUIData());
			m_renderer.updateCameraData(&m_camera.getCameraData());

			// --- DRAW --- //
			m_renderer.updateFrame();

			// --- Polling events, updating IO actions --- //
			m_gui.display();
			m_window.swapBuffers();
		}
	}

	void MAREngine::shutdown() {
		m_gui.shutdown();
		m_renderer.closeRenderer();
		m_window.shutdown();
	}
}