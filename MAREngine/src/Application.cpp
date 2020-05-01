
#include "Application.h"

namespace mar {
	namespace callbacks {
		GLFWwindow* window; Camera* camera;
		int width; int height;
		float lastX; float lastY;
		bool firstMouse;
		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void setCallbacks(GLFWwindow* wind, Camera* cam, const int& w, const int& h);
	}

	int Application::run() {
		char name[] = "MAREngine";
		int width{ 800 };
		int height{ 600 };
		int frameBufferWidth{ 0 };
		int frameBufferHeight{ 0 };
		const std::string shadersPath = "resources/shaders/basic.shader";
		const std::string texturePath = "resources/textures/mr.jpg";
		char portName[] = "\\\\.\\COM7";
		float r = 0.8f;
		float g = 0.8f;
		float b = 0.8f;
		float a = 0.0f;
		float rChange = 0.05f;
		float gChange = 0.05f;
		float bChange = 0.05f;

		mar::Window window(height, width, name);
		Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

		callbacks::setCallbacks(window.getWindow(), &camera, width, height);

		float vertices[] = {
			// Vertex Pos (x, y, z) // Texture Coords
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, // first rectangle
			 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, // second
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, // third
			-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, // fourth
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, // fifth
			 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, // sixth
			 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f
		};

		std::vector<glm::vec3> cubePositions = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.5f, 2.5f, -7.5f),
			glm::vec3(-1.5f, -0.5f, -4.5f)
		};

		Renderer renderer(sizeof(vertices) / sizeof(vertices[0]));
		Shader shader(shadersPath);

		VertexArray va;

		VertexBuffer vb(sizeof(vertices), vertices);

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

		SerialPortMonitor spm(portName);
		spm.start();

		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 transform;

		while (!glfwWindowShouldClose(window.getWindow())) {
			// --- Processing Input --- //
			camera.processInput(window.getWindow());
			//camera.processInput(window.getWindow());

			// --- Rendering, binding textures, creating matrix transformations --- //
			renderer.clear();
			shader.bind();
			va.bind();

			shader.setUniform4f("u_Color", r, g, b, a);

			projection = glm::perspective(glm::radians(camera.getZoom()), (float)width / (float)height, 0.1f, 100.0f);
			shader.setUniformMat4f("u_Projection",projection);
			view = camera.getViewMatrix();
			shader.setUniformMat4f("u_View", view);
			
			float differentAngle = 0.0f;

			for (auto const& cubePosition : cubePositions) {
				model = glm::translate(glm::mat4(1.0f), cubePosition);
				float angle = 20.0f * (differentAngle++);
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				shader.setUniformMat4f("u_Model", model);

				transform = glm::translate(glm::mat4(1.0f), cubePosition);
				transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(spm.getX(), spm.getY(), spm.getZ()));
				shader.setUniformMat4f("u_Transform", transform);

				renderer.draw();
			}

			rgbColorsChange(r, g, b, rChange, gChange, bChange);

			// --- Polling events, updating IO actions --- //
			window.swapBuffers();
		}

		return 0;
	}

	void Application::rgbColorsChange(float& r, float& g, float& b, float& rc, float& gc, float& bc) {
		if (r > 1.0f) rc = -0.5f;
		else if (r < 0.0f) rc = 0.05f;

		if (g > 1.0f) gc = -0.5f;
		else if (g < 0.0f) gc = 0.05f;

		r += rc;
		g += gc;
	}

	namespace callbacks {
		void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
			if (firstMouse) {
				lastX = (float)xpos;
				lastY = (float)ypos;
				firstMouse = false;
			}

			float xoffset = (float)xpos - lastX;
			float yoffset = lastY - (float)ypos;

			lastX = (float)xpos;
			lastY = (float)ypos;

			camera->processMouseMovement(xoffset, yoffset);
		}

		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			camera->processMouseScroll(yoffset);
		}

		void setCallbacks(GLFWwindow* wind, Camera* cam, const int& w, const int& h) {
			window = wind;
			camera = cam;
			width = w;
			height = h;
			lastX = (float)width / 2.0f;
			lastY = (float)height / 2.0f;
			firstMouse = true;

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
		}
	}
}