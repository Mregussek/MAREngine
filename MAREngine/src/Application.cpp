/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		std::vector<std::tuple<std::shared_ptr<Shapes>, glm::vec3, glm::vec3, std::string>> shapes;
		shapes.push_back(std::make_tuple<std::shared_ptr<Shapes>, glm::vec3, glm::vec3>(std::make_shared<Cube>(), { 0.0f,  0.0f,  0.0f }, { 0.0f, 0.0f, 0.0f }, "resources/textures/mr.jpg"));
		shapes.push_back(std::make_tuple<std::shared_ptr<Shapes>, glm::vec3, glm::vec3>(std::make_shared<Cube>(), { 3.0f,  2.0f, -7.5f }, { 0.0f, 0.0f, 0.0f }, "resources/textures/wall.jpg"));
		shapes.push_back(std::make_tuple<std::shared_ptr<Shapes>, glm::vec3, glm::vec3>(std::make_shared<Cube>(), {-3.0f, -2.0f, -7.5f }, { 0.0f, 0.0f, 0.0f }, "resources/textures/wall.jpg"));
		shapes.push_back(std::make_tuple<std::shared_ptr<Shapes>, glm::vec3, glm::vec3>(std::make_shared<Cube>(), {-1.5f,  2.0f, -2.5f }, { 0.0f, 0.0f, 0.0f }, "resources/textures/mr.jpg"));

		Camera camera(width, height);
		mar::Window window(height, width, name, &camera);
		GUI gui(&window, glsl_version);

		Renderer renderer;
		renderer.initializeRenderer(std::make_shared<RendererOpenGLFactory>());
		
		Shader shader(shadersPath);
		
		for (auto const& s : shapes) {
			std::shared_ptr<Shapes> shape;
			glm::vec3 center;
			glm::vec3 angle;
			std::string texture;

			std::tie(shape, center, angle, texture) = s;
			renderer.pushObject(shape, center, texture);
			gui.push(center, { 0.0f, 0.0f, 0.0f });
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