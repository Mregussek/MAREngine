#include "Shader.h"

Shader::Shader(const std::string& filePath)
	: _filePath(filePath),
	_rendererId(0)
{
	_programSource = parseShader();
	_rendererId = createShader();
}

Shader::~Shader() {
	glDeleteProgram(_rendererId);
}

void Shader::bind() const {
	glUseProgram(_rendererId);
}

void Shader::unbind() const {
	glUseProgram(0);
}

void Shader::setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
	glUniform4f(getUniformLocation(name), red, green, blue, alpha);
}

unsigned int Shader::getUniformLocation(const std::string& name) {
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		return _uniformLocationCache[name];

	int location = glGetUniformLocation(_rendererId, name.c_str());
	if (location == -1)
		std::cout << "Warning: Uniform " << name << " doesnt exist!\n";

	return location;
}

Shader::ShaderProgramSource Shader::parseShader() {
	enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

	std::ifstream stream(_filePath);
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
	unsigned int _rendererId = glCreateShader(type);
	const char* src = sourceCode.c_str();
	glShaderSource(_rendererId, 1, &src, nullptr);
	glCompileShader(_rendererId);

	int result;
	glGetShaderiv(_rendererId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(_rendererId, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length * sizeof(char)];
		glGetShaderInfoLog(_rendererId, length, &length, message);
		std::cout << "Failed to compile shader: "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< message << std::endl;
		glDeleteShader(_rendererId);
		delete[] message;
		return 0;
	}

	return _rendererId;
}

unsigned int Shader::createShader() {
	unsigned int shaderProgramId = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, _programSource._vertexSource);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, _programSource._fragmentSource);

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