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

	//std::cout << "VERTEX\n";
	//std::cout << vector[0] << std::endl;
	//std::cout << "FRAGMENT\n";
	//std::cout << vector[1] << std::endl;

	return { vector[0], vector[1] };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& sourceCode) {
	unsigned int shaderId = glCreateShader(type);
	const char* src = sourceCode.c_str();
	glShaderSource(shaderId, 1, &src, nullptr);
	glCompileShader(shaderId);

	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(shaderId, length, &length, message);
		std::cout << "Failed to compile shader: "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< message << std::endl;
		glDeleteShader(shaderId);
		delete[] message;
		return 0;
	}

	return shaderId;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int shaderProgramId = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(shaderProgramId, vs);
	glAttachShader(shaderProgramId, fs);
	glLinkProgram(shaderProgramId);
	glValidateProgram(shaderProgramId);

	int result;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shaderProgramId, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length * sizeof(char)];
		glGetProgramInfoLog(shaderProgramId, length, &length, message);
		std::cout << "Failed to link shaders: "
			<< message << std::endl;
		delete[] message;
		return 0;
	}

	glUseProgram(shaderProgramId);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return shaderProgramId;
}