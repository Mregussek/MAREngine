#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

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

	// some init setup for opengl (DONT WORK WITH CORE_PROFILE)
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
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f  // 3
	};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		1, 2, 3 // second triangle
	};

	VertexBuffer vb(sizeof(positions), positions);
	ElementBuffer ib(indices, 6);

	VertexBufferLayout layout;
	layout.push<float>(2);

	VertexArray va;
	va.addBuffer(vb, layout);

	auto source = Shader::parseShader(shadersPath);
	auto shader = Shader::createShader(source._vertexSource, source._fragmentSource);

	int location = glGetUniformLocation(shader, "u_Color");
	assert(location != -1);
	glUniform4f(location, r, g, b, a);

	va.unbind();

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	while (!glfwWindowShouldClose(window)) { // main loop - render loop
		// check for ESC press
		processInput(window);

		// --- Rendering
		glClear(GL_COLOR_BUFFER_BIT); 

		glUseProgram(shader);
		glUniform4f(location, r, g, b, a);

		va.bind();
		ib.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		rgbColorsChange(r, g, b, rChange, gChange, bChange);

		// --- Update input (poll IO events(keys pressed/ released, mouse moved etc.))
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteProgram(shader);

	// end of program
	glfwTerminate();

	return 0;
}

int main() {
	return chernoCourse();
}