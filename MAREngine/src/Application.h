// Vertices
#include "VertexBuffer/VertexBuffer.h"
#include "VertexBuffer/VertexBufferLayout.h"
#include "ElementBuffer/ElementBuffer.h"
#include "VertexArray/VertexArray.h"
// Rendering
#include "Shader/Shader.h"
#include "Renderer/Renderer.h"
#include "Texture/Texture.h"
#include "Camera/Camera.h"
// Window && Input
#include "Window/Window.h"
#include "Devices/SerialPortMonitor.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int width{ 800 };
int height{ 600 };

float lastX = (float)width / 2.0f;
float lastY = (float)height / 2.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_size_callback);
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	glfwSetScrollCallback(window.getWindow(), scroll_callback);

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
		glm::vec3(0.0f,  0.0f,  0.0f)
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

	//SerialPortMonitor spm(portName);
	//spm.start();

	while (!glfwWindowShouldClose(window.getWindow())) {
		// --- Processing Input --- //
		//window.processInput(window.getWindow(), spm);
		camera.processInput(window.getWindow());

		// --- Rendering --- //
		renderer.clear();
		texture.bind();
		shader.bind();

		glm::mat4 cameraProjection = glm::perspective(glm::radians(camera.getZoom()), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 cameraView = camera.GetViewMatrix();

		va.bind();
		float differentAngle = 0.0f;

		for (auto const& cubePosition : cubePositions) {
			glm::mat4 cube = glm::translate(glm::mat4(1.0f), cubePosition);
			float angle = 20.0f * (differentAngle++);
			glm::mat4 rotationCubeModel = glm::rotate(cube, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			glm::mat4 mvp = cameraProjection * rotationCubeModel * cameraView;

			shader.setUniform4f("u_Color", r, g, b, a);
			shader.setUniformMat4f("u_MVP", mvp);
			
			renderer.draw();
		}

		rgbColorsChange(r, g, b, rChange, gChange, bChange);

		// --- Polling events, updating IO actions --- //
		window.swapBuffers();
	}

	return 0;
}

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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll((float)yoffset);
}

/* ----------------------- For M letter positions and indices!!!
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