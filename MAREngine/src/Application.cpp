/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		// --- STARTUP SHAPES FOR APPLCATION WITH ITS CENTERS, ANGLES AND TEXTURES --- //
		Scene scene;
		///! TODO: When scene is starting as empty, we cannot see newly added objects!
		scene.initializeScene(SceneType::DEFAULT);
		std::vector<Shapes> shapes = scene.getShapes();
		std::vector<glm::vec3> centers = scene.getCenters();
		std::vector<glm::vec3> angles = scene.getAngles();
		std::vector<std::string> textures = scene.getTextures();
		
		// --- INITIALIZATION PROCESS --- //
		Camera camera(width, height);
		Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);
		Renderer renderer;
		renderer.createRenderer(std::make_shared<RendererOpenGLFactory>());

		// --- PREPARE SCENE FOR DISPLAY --- //
		for (unsigned int i = 0; i < shapes.size(); i++) {
			renderer.pushObject(&shapes[i], centers[i], textures[i]);
			gui.push(centers[i], angles[i]);
		}
			
		renderer.initialize();
		//gui.connectToRenderer(&renderer);
		renderer.unbind();
		
		// --- MAIN LOOP (RENDER LOOP) --- //
		while (window.shouldClose()) {
			// --- Processing Input --- //
			{ // update for every frame
				camera.processInput(window.getWindow());
				gui.prepareNewFrame();
			}

			// --- Renderer Setup before drawing --- //
			{
				renderer.setGUIvectors(gui.getCentersVector(), gui.getAnglesVector());
				renderer.setGUImatrices(gui.getColors(), gui.getTranslationMatrix(), gui.getRotationMatrix());
				renderer.setCameraMatrices(camera.getProjectionMatrix(), camera.getViewMatrix(), camera.getModelMatrix());
				renderer.setCameraVectors(camera.getCameraPosition());
			}

			{
				renderer.updateFrame();
			}

			// --- Polling events, updating IO actions --- //
			{ // display gui on screen and swap buffers
				gui.display();
				window.swapBuffers();
			}
		}

		renderer.closeRenderer();

		return 0;
	}
}