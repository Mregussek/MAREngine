/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		mar::Window window(height, width, name);
		GUI gui(&window, glsl_version);
		Camera camera(width, height);

		callbacks::setCallbacks(window.getWindow(), &camera); // for mouse usage

		std::vector<glm::vec3> positions = {
			{0.0f, 0.0f, 0.0f},
			{3.0f, 2.0f, -7.5f}
		};

		Shader shader(shadersPath);

		std::vector<Cube> cubes = { Cube(), Cube() };

		assert(positions.size() == cubes.size());
		for (unsigned int i = 0; i < cubes.size(); i++) {
			Mesh::changeCenterOfObject(cubes[i].vertices, cubes[i].getSizeofVertices(), cubes[i].getStride(), positions[i], cubes[i].verticesVector);
			cubes[i].prescribeCenter(positions[i]);
		}

		Mesh mesh(cubes[0]); // need only one cube

		Texture texture(texturePath);
		texture.bind();

		shader.bind();
		shader.setUniform1i("u_Texture", 0);

		shader.unbind();
		mesh.unbind();

		Renderer renderer(mesh.sizeofVertices(), mesh.sizeofIndices());

		while (window.shouldClose()) {
			// --- Processing Input --- //
			camera.processInput(window.getWindow());
			gui.prepareNewFrame();

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
			}
			
			for(auto const& c : cubes)	{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), c.getCenter());
				shader.setUniformMat4f("u_Model", model);
				shader.setUniformMat4f("u_Transform", camera.getRotateMatrixOnPress(c.getCenter()));
				shader.setUniformMat4f("u_GUIrotation", gui.getRotationMatrix());

				renderer.draw();
			}

			// --- Polling events, updating IO actions --- //
			gui.display();
			window.swapBuffers();
		}

		return 0;
	}
}