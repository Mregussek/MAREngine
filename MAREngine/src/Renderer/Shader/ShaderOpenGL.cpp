/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShaderOpenGL.h"

namespace mar {
	void ShaderOpenGL::initialize(const std::string& filePath) {
		_filePath = filePath;
		_rendererId = 0;
		_programSource = parseShader();
		_rendererId = createShader();
	}

	void ShaderOpenGL::shutdown() {
		glDeleteProgram(_rendererId);
	}

	void ShaderOpenGL::bind() const {
		glUseProgram(_rendererId);
	}

	void ShaderOpenGL::unbind() const {
		glUseProgram(0);
	}

	void ShaderOpenGL::setUniformSampler2D(const std::string& name, std::vector<int> sampler) {
		int* s = new int[sampler.size()];
		std::copy(sampler.begin(), sampler.end(), s);
		glUniform1iv(getUniformLocation(name), sampler.size(), s);
		delete[] s;
	}

	void ShaderOpenGL::setUniformVectorMat4(const std::string& name, std::vector<glm::mat4> sampler) {
		glm::mat4* s = new glm::mat4[sampler.size()];
		std::copy(sampler.begin(), sampler.end(), s);
		glUniformMatrix4fv(getUniformLocation(name), sampler.size(), GL_FALSE, glm::value_ptr(*s));
		delete[] s;
	}

	void ShaderOpenGL::setUniformMat4f(const std::string& name, const glm::mat4& matrix4x4) {
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix4x4));
	}

	void ShaderOpenGL::setUniformVector3(const std::string& name, const glm::vec3& vector3) {
		glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vector3));
	}

	void ShaderOpenGL::setUniform1i(const std::string& name, int value) {
		glUniform1i(getUniformLocation(name), value);
	}

	void ShaderOpenGL::setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
		glUniform4f(getUniformLocation(name), red, green, blue, alpha);
	}

	void ShaderOpenGL::setUniform4fv(const std::string& name, const float* floats4) {
		glUniform4fv(getUniformLocation(name), 1, floats4);
	}

	int ShaderOpenGL::getUniformLocation(const std::string& name) {
		if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
			return _uniformLocationCache[name];

		int location = glGetUniformLocation(_rendererId, name.c_str());
		if (location == -1)
			std::cout << "Warning: Uniform " << name << " doesnt exist!\n";

		return location;
	}

	ShaderProgramSource ShaderOpenGL::parseShader() {
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

		return { vector[0], vector[1] };
	}

	unsigned int ShaderOpenGL::compileShader(unsigned int type, const std::string& sourceCode) {
		unsigned int _rendererId = glCreateShader(type);
		const char* src = sourceCode.c_str();
		glShaderSource(_rendererId, 1, &src, nullptr);
		glCompileShader(_rendererId);

		int result;
		glGetShaderiv(_rendererId, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) {
			int length = 100;
			char message[100];
			glGetShaderInfoLog(_rendererId, length, &length, message);
			std::cout << "Failed to compile shader: "
				<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< message << std::endl;
			glDeleteShader(_rendererId);
			return 0;
		}

		return _rendererId;
	}

	unsigned int ShaderOpenGL::createShader() {
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
			int length = 100;
			char message[100];
			glGetProgramInfoLog(shaderProgramId, length, &length, message);
			std::cout << "Failed to link shaders: "
				<< message << std::endl;
			return 0;
		}

		glUseProgram(shaderProgramId);

		glDeleteShader(vs);
		glDeleteShader(fs);

		return shaderProgramId;
	}
}