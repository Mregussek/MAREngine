#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "Debug.h"

void frameBuffer_SizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void rgbColorsChange(float& r, float& g, float& b, float& rc, float& gc, float& bc) {
	if (r > 1.0f) rc = -0.5f;
	else if (r < 0.0f) rc = 0.05f;

	if (g > 1.0f) gc = -0.5f;
	else if (g < 0.0f) gc = 0.05f;

	//if (b > 1.0f) bc = -0.5f;
	//else if (b < 0.0f) bc = 0.05f;

	r += rc;
	g += gc;
	//b += bc;
}

int chernoCourse() {
	const char* name = "MAREngine";
	const int width{ 640 };
	const int height{ 480 };
	int frameBufferWidth{ 0 };
	int frameBufferHeight{ 0 };
	const std::string shadersPath = "resources/shaders/basic.shader";
	float r = 0.2f;
	float g = 0.6f;
	float b = 0.8f;
	float a = 1.0f;
	float rChange = 0.05f;
	float gChange = 0.05f;
	float bChange = 0.05f;

	// init glfw
	if(!glfwInit()) return -1;

	// some init setup for opengl
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create window
	GLFWwindow* window{ glfwCreateWindow(width, height, name, nullptr, nullptr) };
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // important!!!
	glfwSetFramebufferSizeCallback(window, frameBuffer_SizeCallback); // we want to call frameBuffer_SizeCallback on every window resize by registering it
	glfwSwapInterval(1);

	// init glew (needs window and opengl context)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = { 
			-0.7f,  -0.5f,  // 0
			-0.35f, -0.5f,  // 1
			-0.2f,   0.5f,  // 2
			-0.55f,  0.5f,  // 3

			 0.7f,  -0.5f,  // 4
			 0.35f, -0.5f,  // 5
			 0.2f,   0.5f,  // 6
			 0.55f,  0.5f,  // 7

			-0.01f,  0.25f, // 8
			 0.01f,  0.25f, // 9
			-0.01f, -0.15f, // 10
			 0.01f, -0.15f  // 11
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
	
	VertexBuffer vb(sizeof(positions), positions);
	ElementBuffer eb(indices, sizeof(indices) / sizeof(indices[0]));

	VertexBufferLayout layout;
	layout.push<float>(2);

	VertexArray va;
	va.addBuffer(vb, layout);
	
	Shader shader(shadersPath);
	shader.bind();
	shader.setUniform4f("u_Color", r, g, b, a);

	va.unbind();
	shader.unbind();
	vb.unbind();
	eb.unbind();

	Renderer renderer(sizeof(indices) / sizeof(indices[0]));

	while (!glfwWindowShouldClose(window)) { // main loop - render loop
		// check for ESC press
		processInput(window);

		// --- Rendering
		renderer.clear();

		shader.setUniform4f("u_Color", r, g, b, a);

		renderer.draw(va, eb, shader);

		rgbColorsChange(r, g, b, rChange, gChange, bChange);

		// --- Update input (poll IO events(keys pressed/ released, mouse moved etc.))
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// end of program
	glfwTerminate();

	return 0;
}

int main() {
	return chernoCourse();
}