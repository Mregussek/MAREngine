#include "Shader.h"

Shader::ShaderProgramSource Shader::parseShader(const std::string& filePath) {
	enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

	std::ifstream stream(filePath);
	std::string line;
	std::vector<std::string> vector(2);
	auto type = ShaderType::None;

	while (std::getline(stream, line))
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
			else if (line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
		}
		else vector[(int)type] += line + "\n";

	std::cout << "VERTEX\n";
	std::cout << vector[0] << std::endl;
	std::cout << "FRAGMENT\n";
	std::cout << vector[1] << std::endl;

	return { vector[0], vector[1] };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
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

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
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