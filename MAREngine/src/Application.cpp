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
			, Cube()
			, Pyramid()
		};
		std::vector<glm::vec3> centers = {
			{ 0.0f,  0.0f,  0.0f }
			, { 3.0f,  2.0f, -7.5f }
			, {-3.0f, -2.0f, -7.5f }
			, {-1.5f,  2.0f, -2.5f }
		};
		std::vector<glm::vec3> angles = {
			 { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
		};
		std::vector<std::string> textures = {
			"resources/textures/mr.jpg"
			, "resources/textures/wall.jpg"
			, "resources/textures/blue-texture.jpg"
			, "resources/textures/red-texture.jpg"
		};

		Camera camera(width, height);
		Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);

		Renderer renderer;
		renderer.initializeRenderer(std::make_shared<RendererOpenGLFactory>());
		
		for (unsigned int i = 0; i < shapes.size(); i++) {
			renderer.pushObject(&shapes[i], centers[i], textures[i]);
			gui.push(centers[i], angles[i]);
		}
			
		renderer.initialize(shadersPath);
		renderer.unbind();
		
		// --- Main Loop --- //
		while (window.shouldClose()) {
			// --- Processing Input --- //
			{ // update for every frame
				camera.processInput(window.getWindow());
				gui.prepareNewFrame();
				renderer.updateFrame(gui.getCentersVector(), gui.getAnglesVector());
			}
			
			// --- Rendering, binding textures, creating matrix transformations --- //
			{ // Prepare for rendering
				renderer.clear();
				renderer.bind();
			}

			{ // Setup shaders (these, which are the same for all objects)
				renderer.setGUImatrices(gui.getColors(), gui.getTranslationMatrix(), gui.getRotationMatrix());
				renderer.setCameraMatrices(camera.getProjectionMatrix(), camera.getViewMatrix(), camera.getModelMatrix());
				renderer.setRenderMatrices();
			}

			{ // One render call
				renderer.draw();
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