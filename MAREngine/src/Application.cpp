/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		// --- STARTUP SHAPES FOR APPLCATION WITH ITS CENTERS, ANGLES AND TEXTURES --- //
		std::vector<Shapes> shapes = {
			Cube()
			, Pyramid()
			, Surface()
			, Wall()
			, Cube()
			, Pyramid()
		};
		std::vector<glm::vec3> centers = {
			{ 0.0f,  0.0f,  0.0f }
			, { 3.0f,  0.0f, -4.5f }
			, { 0.0f,  -0.025f,  0.0f }
			, { -4.0f,  -0.025f,  0.0f }
			, {-3.0f,  0.0f, -4.5f }
			, {-1.5f,  0.0f, -2.5f }
		};
		std::vector<glm::vec3> angles = {
			 { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
		};
		std::vector<std::string> textures = {
			mrTex
			, yellowTex
			, grassTex
			, wallTex
			, blueTex
			, redTex
		};

		// --- INITIALIZATION PROCESS --- //
		Camera camera(width, height);
		Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);
		Renderer renderer;
		gui.connectToRenderer(&renderer);
		renderer.createRenderer(std::make_shared<RendererOpenGLFactory>());
		
		for (unsigned int i = 0; i < shapes.size(); i++) {
			renderer.pushObject(&shapes[i], centers[i], textures[i]);
			gui.push(centers[i], angles[i]);
		}
			
		renderer.initialize(shadersPath);
		renderer.unbind();
		
		// --- MAIN LOOP --- //
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