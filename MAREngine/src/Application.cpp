/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		// --- PREPARE FOR INITIALIZATION PROCESS (CREATE ALL NEEDED OBJECTS) --- //
		Camera camera(width, height);
		Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);
		Renderer renderer;
		Scene scene;
		
		// --- INITIALIZATION PROCESS --- //
		scene.initializeScene(SceneType::DEFAULT);
		renderer.createRenderer(std::make_shared<RendererOpenGLFactory>());

		// --- PUSH SCENE TO RENDERER AND SET GUI --- //
		for (unsigned int i = 0; i < scene.getShapesNumber(); i++) {
			renderer.pushObject(scene.getShape(i), scene.getCenter(i), scene.getTexture(i));
			gui.push(scene.getCenter(i), scene.getAngle(i));
		}
			
		// --- INITIALIZE RENDERER WITH SCENE AND PREPARE FOR RENDERING --- //
		renderer.initialize();
		gui.connectToRenderer(&renderer);
		renderer.unbind();
		
		// --- MAIN LOOP (RENDER LOOP) --- //
		while (window.shouldClose()) {
			// --- Processing Input --- //
			camera.processInput(window.getWindow());
			gui.prepareNewFrame();

			// --- Renderer Setup before drawing --- //
			renderer.setGUIvectors(gui.getCentersVector(), gui.getAnglesVector());
			renderer.setGUImatrices(gui.getColors(), gui.getTranslationMatrix(), gui.getRotationMatrix());
			renderer.setCameraMatrices(camera.getProjectionMatrix(), camera.getViewMatrix(), camera.getModelMatrix());
			renderer.setCameraVectors(camera.getCameraPosition());

			// --- DRAW --- //
			renderer.updateFrame();

			// --- Polling events, updating IO actions --- //
			gui.display();
			window.swapBuffers();
		}

		renderer.closeRenderer();

		return 0;
	}
}