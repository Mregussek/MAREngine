// Vertices
#include "VertexBuffer/VertexBuffer.h"
#include "VertexBuffer/VertexBufferLayout.h"
#include "ElementBuffer/ElementBuffer.h"
#include "VertexArray/VertexArray.h"
// Rendering
#include "Shader/Shader.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"
// Window && Input
#include "Window/Window.h"
#include "Window/SerialPortMonitor.h"

void rgbColorsChange(float& r, float& g, float& b, float& rc, float& gc, float& bc) {
	if (r > 1.0f) rc = -0.5f;
	else if (r < 0.0f) rc = 0.05f;

	if (g > 1.0f) gc = -0.5f;
	else if (g < 0.0f) gc = 0.05f;

	r += rc;
	g += gc;
}

int run() {
	char name[] = "MAREngine";
	int width{ 640 };
	int height{ 480 };
	int frameBufferWidth{ 0 };
	int frameBufferHeight{ 0 };
	const std::string shadersPath = "resources/shaders/basic.shader";
	const std::string texturePath = "resources/textures/mr.jpg";
	char portName[] = "\\\\.\\COM7";
	float r = 0.2f;
	float g = 0.6f;
	float b = 0.8f;
	float a = 0.0f;
	float rChange = 0.05f;
	float gChange = 0.05f;
	float bChange = 0.05f;

	Window window(height, width, name);

	/*
	float positions[] = {
			// x    // y   // z
			-0.7f,  -0.5f,  0.0f, // vertex 0
			-0.35f, -0.5f,  0.0f, // vertex 1
			-0.2f,   0.5f,  0.0f, // vertex 2
			-0.55f,  0.5f,  0.0f, // vertex 3

			 0.7f,  -0.5f,  0.0f, // vertex 4
			 0.35f, -0.5f,  0.0f, // vertex 5
			 0.2f,   0.5f,  0.0f, // vertex 6
			 0.55f,  0.5f,  0.0f, // vertex 7

			-0.01f,  0.25f, 0.0f, // vertex 8
			 0.01f,  0.25f, 0.0f, // vertex 9
			-0.01f, -0.15f, 0.0f, // vertex 10
			 0.01f, -0.15f, 0.0f  // vertex 11
	};

	unsigned int indices[] = {
		0, 1, 2, // LeftArm first triangle
		2, 3, 0, // LeftArm second triangle

		4, 5, 6, // RightArm first triangle
		4, 6, 7, // RightArm second triangle

		3, 2, 10, // LeftCenterArm first triangle
		2, 8, 10, // LefCentertArm second triangle

		9, 6, 11, // RightCenterArm first triangle
		11, 6, 7, // RightCentertArm second triangle

		8, 9, 11, // Center first
		8, 10, 11 // Center second
	};
	*/

	float positions[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3 ,0
	};

	Renderer renderer(sizeof(positions) / sizeof(positions[0]), sizeof(indices) / sizeof(indices[0]));
	Shader shader(shadersPath);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VertexArray va;

	VertexBuffer vb(sizeof(positions), positions);
	//ElementBuffer eb(indices, sizeof(indices));

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

	//SerialPortMonitor spm(portName);
	//spm.start();

	while (!glfwWindowShouldClose(window.getWindow())) {
		// --- Processing Input --- //
		//window.processInput(spm);
		window.processInput();

		// --- Rendering --- //
		renderer.clear();
		texture.bind();
		shader.bind();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

		shader.setUniform4fv("u_Model", model);
		shader.setUniform4fv("u_Projection", projection);
		shader.setUniform4fv("u_View", view);

		shader.setUniform4f("u_Color", r, g, b, a);

		va.bind();
		renderer.draw();

		rgbColorsChange(r, g, b, rChange, gChange, bChange);

		// --- Polling events, updating IO actions --- //
		window.swapBuffers();
	}

	return 0;
}