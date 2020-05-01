/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Application.h"

namespace mar {
	int Application::run() {
		char name[] = "MAREngine";
		int width{ 1280 };
		int height{ 720 };
		const std::string shadersPath = "resources/shaders/basic.shader";
		const std::string texturePath = "resources/textures/mr.jpg";
		char portName[] = "\\\\.\\COM7";
		const char* glsl_version = "#version 460";
		float r = 0.8f;
		float g = 0.8f;
		float b = 0.8f;
		float a = 0.0f;
		float rChange = 0.05f;
		float gChange = 0.05f;
		float bChange = 0.05f;

		mar::Window window(height, width, name);
		GUI gui(&window, glsl_version);
		Camera camera(width, height);

		callbacks::setCallbacks(window.getWindow(), &camera); // for mouse usage
		
		float vertices[] = {
			//  front (x, y, z)		// Texture
			-1.0f, -1.0f,  1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,	1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,	1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,	0.0f, 1.0f,
			//  back 
			-1.0f, -1.0, -1.0f,		0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,	1.0f, 0.0f,
			 1.0f,  1.0, -1.0f,		1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,	0.0f, 1.0f,
		};

		unsigned int elements[] = {
			// front	// back
			0, 1, 2,	7, 6, 5,
			2, 3, 0,	5, 4, 7,
			// right	// left
			1, 5, 6,	4, 0, 3,
			6, 2, 1,	3, 7, 4,
			// bottom	// top
			4, 5, 1,	3, 2, 6,
			1, 0, 4,	6, 7, 3
		};

		std::vector<glm::vec3> cubes = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.5f, 0.5f, -7.5f),
			glm::vec3(-1.5f, -0.5f, -4.5f)
		};

		Shader shader(shadersPath);

		VertexArray va;

		VertexBuffer vb(sizeof(vertices), vertices);
		ElementBuffer eb(sizeof(elements), elements);
		VertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(2);
		va.addBuffer(vb, layout);

		Texture texture(texturePath);
		texture.bind();

		shader.bind();
		shader.setUniform1i("u_Texture", 0);

		va.unbind();
		shader.unbind();
		vb.unbind();
		eb.unbind();

		Renderer renderer(vb.getSize(), eb.getIndicesNumber());

		//SerialPortMonitor spm(portName);
		//spm.start();

		while (window.shouldClose()) {
			// --- Processing Input --- //
			camera.processInput(window.getWindow());
			gui.prepareNewFrame();

			// --- Rendering, binding textures, creating matrix transformations --- //
			renderer.clear();
			shader.bind();
			va.bind();
			eb.bind();

			shader.setUniform4fv("u_Color", gui.getColors());
			shader.setUniformMat4f("u_Projection", camera.getProjectionMatrix());
			shader.setUniformMat4f("u_View", camera.getViewMatrix());
			shader.setUniformMat4f("u_GUItranslation", gui.getTranslationMatrix());
			
			float differentAngle = 0.0f;
			for (auto const& cubePosition : cubes) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePosition);
				float angle = 20.0f * (differentAngle++);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader.setUniformMat4f("u_Model", model);

				//transform = camera.getRotateMatrixSPM(cubePosition, glm::vec3(spm.getY(), spm.getX(), spm.getZ()));
				glm::mat4 transform = camera.getRotateMatrixOnPress(cubePosition);
				shader.setUniformMat4f("u_Transform", transform);

				shader.setUniformMat4f("u_GUIrotation", gui.getRotationMatrix(cubePosition));
				
				renderer.draw();
			}

			// --- Polling events, updating IO actions --- //
			gui.display();
			window.swapBuffers();
		}

		return 0;
	}
}