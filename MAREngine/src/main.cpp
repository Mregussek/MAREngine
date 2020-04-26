#include "mre.h"

struct ShaderProgramSource {
	std::string _vertexSource;
	std::string _fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath) {
	enum class ShaderType {
		None = -1, Vertex = 0, Fragment = 1
	};

	std::ifstream stream(filePath);
	std::string line;
	std::vector<std::string> vector(2);
	auto type = ShaderType::None;

	while (std::getline(stream, line)) 
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
		} else vector[(int)type] += line + "\n";

	return { vector[0], vector[1] };
}

static unsigned int compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader: " 
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< message << std::endl;
		glDeleteShader(id);
		delete[] message;
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(int argc, char** argv) {
	const char* name = "MAREngine";
	const int width{ 640 };
	const int height{ 480 };
	int frameBufferWidth{ 0 };
	int frameBufferHeight{ 0 };
	const std::string shadersPath = "resources/shaders/basic.shader";

	// init glfw
	if(!glfwInit()) return -1;

	// some init setup for opengl (DONT WORK)
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// create window
	GLFWwindow* window{ glfwCreateWindow(width, height, name, nullptr, nullptr) };

	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // important!!!

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
		-0.5f,  0.5f // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	auto source = parseShader(shadersPath);
	std::cout << "VERTEX\n";
	std::cout << source._vertexSource << std::endl;
	std::cout << "FRAGMENT\n";
	std::cout << source._fragmentSource << std::endl;

	unsigned int shader = createShader(source._vertexSource, source._fragmentSource);
	glUseProgram(shader);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		// clear
		glClear(GL_COLOR_BUFFER_BIT); 

		// draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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