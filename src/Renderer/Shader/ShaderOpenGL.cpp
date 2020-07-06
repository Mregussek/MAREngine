/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShaderOpenGL.h"
#include "../../Debug/Log.h"


namespace mar {
	namespace graphics {


		void ShaderOpenGL::initialize(ShaderType shadertype) {
			if (m_initialized) {
				MAR_CORE_TRACE("Cannot re-initialize once compiled shader!");
				return;
			}

			switch(shadertype) {
			case ShaderType::DEFAULT: m_shaderPath = "resources/shaders/basic.shader";
				break;
			case ShaderType::WITHOUT_GUI: m_shaderPath = "resources/shaders/basicWithoutGUI.shader"; 
				break;
			case ShaderType::CUBEMAP: m_shaderPath = "resources/shaders/cubemap.shader";
				break;
			case ShaderType::CUBEMAP_WITHOUT_GUI: m_shaderPath = "resources/shaders/cubemapWithoutGUI.shader"; 
				break;
			}

			m_id = 0;
			m_programSource = parseShader();
			m_id = createShader();
			m_initialized = true;
		}

		void ShaderOpenGL::shutdown() {
			glDeleteProgram(m_id);
		}

		void ShaderOpenGL::bind() const {
			glUseProgram(m_id);
		}

		void ShaderOpenGL::unbind() const {
			glUseProgram(0);
		}

		void ShaderOpenGL::setUniformSampler(const std::string& name, const std::vector<int>& sampler) {
			glUniform1iv(getUniformLocation(name), sampler.size(), sampler.data());
		}

		void ShaderOpenGL::setUniformVectorVec3(const std::string& name, const std::vector<glm::vec3>& vec) {
			glUniform3fv(getUniformLocation(name), vec.size(), glm::value_ptr(*vec.data()));
		}

		void ShaderOpenGL::setUniformVectorMat4(const std::string& name, const std::vector<glm::mat4>& matrices) {
			glUniformMatrix4fv(getUniformLocation(name), matrices.size(), GL_FALSE, glm::value_ptr(*matrices.data()));
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

		void ShaderOpenGL::setUniform1f(const std::string& name, float value) {
			glUniform1f(getUniformLocation(name), value);
		}

		void ShaderOpenGL::setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
			glUniform4f(getUniformLocation(name), red, green, blue, alpha);
		}

		void ShaderOpenGL::setUniform4fv(const std::string& name, const float* floats4) {
			glUniform4fv(getUniformLocation(name), 1, floats4);
		}

		int ShaderOpenGL::getUniformLocation(const std::string& name) {
			if (m_uniformLocation.find(name) != m_uniformLocation.end())
				return m_uniformLocation[name];

			int location = glGetUniformLocation(m_id, name.c_str());
			if (location == -1)
				std::cout << "Warning: Uniform " << name << " does not exist!\n";

			return location;
		}

		ShaderProgramSource ShaderOpenGL::parseShader() {
			enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

			std::ifstream stream(m_shaderPath);
			std::string line;
			std::vector<std::string> vector(2);
			auto type = ShaderType::None;

			while (std::getline(stream, line))
				if (line.find("#shader") != std::string::npos) {
					if (line.find("vertex") != std::string::npos) type = ShaderType::Vertex;
					else if (line.find("fragment") != std::string::npos) type = ShaderType::Fragment;
				}
				else vector[(int)type] += line + "\n";

			MAR_CORE_TRACE("Shader loaded successfully from source file!");

			return { vector[0], vector[1] };
		}

		unsigned int ShaderOpenGL::compileShader(unsigned int type, const std::string& sourceCode) {
			unsigned int id = glCreateShader(type);
			const char* src = sourceCode.c_str();
			glShaderSource(id, 1, &src, nullptr);
			glCompileShader(id);

			int result;
			glGetShaderiv(id, GL_COMPILE_STATUS, &result);

			if (result == GL_FALSE) {
				int length = 100;
				char message[100];
				glGetShaderInfoLog(id, length, &length, message);
				std::string shadertype = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
				std::string s = "Failed to compile shader: " + shadertype + message;

				MAR_CORE_ERROR(s);

				glDeleteShader(id);
				return 0;
			}

			MAR_CORE_TRACE("Shader compiled successfully!");

			return id;
		}

		unsigned int ShaderOpenGL::createShader() {
			unsigned int shaderProgramId = glCreateProgram();
			unsigned int vs = compileShader(GL_VERTEX_SHADER, m_programSource._vertexSource);
			unsigned int fs = compileShader(GL_FRAGMENT_SHADER, m_programSource._fragmentSource);

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
				std::string s = "Failed to load shader: " + std::string(message);

				MAR_CORE_ERROR(s);
				return 0;
			}

			glUseProgram(shaderProgramId);

			glDeleteShader(vs);
			glDeleteShader(fs);

			MAR_CORE_TRACE("Shader created successfully!");

			return shaderProgramId;
		}


} }