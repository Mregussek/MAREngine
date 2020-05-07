/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {

		std::vector<Shapes> shapes = {
			Cube()
			, Cube()
			, Cube()
			, Cube()
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
			, "resources/textures/wall.jpg"
			, "resources/textures/mr.jpg"
		};

		Camera camera(width, height);
		mar::Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);

		Renderer renderer;
		renderer.initializeRenderer(std::make_shared<RendererOpenGLFactory>());
		
		Shader shader(shadersPath);
		
		for (unsigned int i = 0; i < shapes.size(); i++) {
			renderer.pushObject(&shapes[i], centers[i], textures[i]);
			gui.push(centers[i], angles[i]);
		}
			
		{ // initialize startup positions and textures for objects
			renderer.initializeBuffers();
			shader.bind();
			shader.setUniformSampler2D("u_Texture", renderer.getSamplers());
		}
		
		{ // unbind before starting rendering
			shader.unbind();
			renderer.unbind();
		}

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
				shader.bind();
				renderer.bind();
			}

			{ // Setup shaders (these, which are the same for all objects)
				shader.setUniform4fv("u_GUIcolor", gui.getColors());
				shader.setUniformMat4f("u_GUItranslation", gui.getTranslationMatrix());
				shader.setUniformMat4f("u_GUIrotation", gui.getRotationMatrix());
				shader.setUniformMat4f("u_Projection", camera.getProjectionMatrix());
				shader.setUniformMat4f("u_View", camera.getViewMatrix());
				shader.setUniformMat4f("u_Model", camera.getModelMatrix());
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

		return 0;
	}
}