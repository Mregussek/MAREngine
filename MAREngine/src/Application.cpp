/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		std::vector<Cube> cubes = {
			Cube()
			, Cube()
			, Cube()
			//, Cube()
		};
		std::vector<glm::vec3> positions = {
			{0.0f, 0.0f, 0.0f}
			, { 3.0f, 2.0f, -7.5f}
			, { -3.0f, -2.0f, -7.5f}
			, { -1.5f, 2.0f, -2.5f}
		};
		std::vector<int> samplers;

		mar::Window window(height, width, name);
		GUI gui(&window, glsl_version);
		Camera camera(width, height);

		callbacks::setCallbacks(window.getWindow(), &camera);
		
		Mesh mesh;
		Shader shader(shadersPath);

		if (positions.size() < cubes.size()) { std::cerr << "More cubes than positions\n";  exit(0); }
		for (unsigned int i = 0; i < cubes.size(); i++) {
			mesh.push(&cubes[i], positions[i], texturePaths[i]);
			samplers.push_back((int)cubes[i].getID());
			gui.pushCenter(positions[i]);
		}
			
		{ // initialize startup positions and textures for objects
			mesh.initialize();
			shader.bind();
			shader.setUniformSampler2D("u_Texture", samplers);
		}
		
		{ // unbind before starting rendering
			shader.unbind();
			mesh.unbind();
		}
		
		Renderer renderer(mesh.sizeofVertices(), mesh.sizeofIndices());

		while (window.shouldClose()) {
			// --- Processing Input --- //
			{ // update for every frame
				camera.processInput(window.getWindow());
				gui.prepareNewFrame();
				mesh.onUpdate(gui.getCentersVector());
			}
			
			// --- Rendering, binding textures, creating matrix transformations --- //
			{ // Prepare for rendering
				renderer.clear();
				shader.bind();
				mesh.bind();
			}
			
			{ // Setup shaders (these, which are the same for all objects)
				shader.setUniformMat4f("u_Projection", camera.getProjectionMatrix());
				shader.setUniformMat4f("u_View", camera.getViewMatrix());
				shader.setUniform4fv("u_GUIcolor", gui.getColors());
				shader.setUniformMat4f("u_GUItranslation", gui.getTranslationMatrix());
				shader.setUniformMat4f("u_Transform", camera.getRotateMatrixOnPress(positions[0]));
				shader.setUniformMat4f("u_GUIrotation", gui.getRotationMatrix());
			}
			
			{ // Set main position for the whole scene
				glm::mat4 model = glm::translate(glm::mat4(1.0f), positions[0]);
				shader.setUniformMat4f("u_Model", model);
			}

			{ // One render call, BATCH RENDEREING FEATURE
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