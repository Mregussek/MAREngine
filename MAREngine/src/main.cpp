#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

int main(int argc, char** argv) {
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
	float change = 0.05f;

	// init glfw
	if(!glfwInit()) return -1;

	// some init setup for opengl (DONT WORK WITH CORE_PROFILE)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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
	
	glfwSwapInterval(1);

	// init glew (needs window and opengl context)
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << argv[0] << " failed!" << std::endl;
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
		2, 3, 0 // second triangle
	};

	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.push<float>(2);
	va.addBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	auto source = Shader::parseShader(shadersPath);
	unsigned int shader = Shader::createShader(source._vertexSource, source._fragmentSource);
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color");
	assert(location != -1);
	glUniform4f(location, r, g, b, a);

	va.unbind();

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// clear
		glClear(GL_COLOR_BUFFER_BIT); 

		// draw
		glUseProgram(shader);
		glUniform4f(location, r, g, b, a);

		va.bind();
		ib.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (r > 1.0f) change = -0.5f;
		else if (r < 0.0f) change = 0.05f;

		r += change;
		g -= change;

		// end draw
		glfwSwapBuffers(window);

		// update input
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	// end of program
	glfwTerminate();

	return 0;
}