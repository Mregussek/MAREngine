/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShaderOpenGL.h"
#include "../../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		void ShaderOpenGL::initialize(ShaderType shadertype) {
			if (m_initialized) {
				GRAPHICS_TRACE("SHADER_OPENGL: Cannot re-initialize once compiled shader!");
				return;
			}

			switch(shadertype) {
			case ShaderType::ENTITY_COLOR: m_shaderPath = "resources/shaders/entityColor.shader";
				break;
			case ShaderType::ENTITY_TEXTURE2D: m_shaderPath = "resources/shaders/entityTexture2D.shader";
				break;
			case ShaderType::ENTITY_CUBEMAP: m_shaderPath = "resources/shaders/entityCubemap.shader";
				break;
			case ShaderType::BATCH_TEXTURE: m_shaderPath = "resources/shaders/BatchTexture.shader";
				break;
			}

			m_id = 0;
			m_programSource = parseShader();
			m_id = createShader();
			m_initialized = true;
		}

		void ShaderOpenGL::shutdown() {
			MAR_CORE_GL_FUNC(glDeleteProgram(m_id));
		}

		void ShaderOpenGL::bind() const {
			MAR_CORE_GL_FUNC(glUseProgram(m_id));
		}

		void ShaderOpenGL::unbind() const {
			MAR_CORE_GL_FUNC(glUseProgram(0));
		}

		void ShaderOpenGL::setUniformInt(const std::string& name, const std::vector<int32_t>& ints) {
			MAR_CORE_GL_FUNC( glUniform1iv(getUniformLocation(name), ints.size(), ints.data()) );
		}

		void ShaderOpenGL::setUniformSampler(const std::string& name, const std::vector<int>& sampler) {
			MAR_CORE_GL_FUNC( glUniform1iv(getUniformLocation(name), sampler.size(), sampler.data()) );
		}

		void ShaderOpenGL::setUniformVectorVec3(const std::string& name, const std::vector<maths::vec3>& vec) {
			MAR_CORE_GL_FUNC( glUniform3fv(getUniformLocation(name), vec.size(),  maths::vec3::value_ptr(vec)) );
		}

		void ShaderOpenGL::setUniformVectorMat4(const std::string& name, const std::vector<maths::mat4>& matrices) {
			MAR_CORE_GL_FUNC( glUniformMatrix4fv(getUniformLocation(name), matrices.size(), GL_FALSE, maths::mat4::value_ptr(matrices)) );
		}

		void ShaderOpenGL::setUniformMat4f(const std::string& name, const maths::mat4& matrix4x4) {
			MAR_CORE_GL_FUNC( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, maths::mat4::value_ptr(matrix4x4)) );
		}

		void ShaderOpenGL::setUniformVector3(const std::string& name, const maths::vec3& vector3) {
			MAR_CORE_GL_FUNC( glUniform3fv(getUniformLocation(name), 1, maths::vec3::value_ptr(vector3)) );
		}

		void ShaderOpenGL::setUniform1i(const std::string& name, int value) {
			MAR_CORE_GL_FUNC( glUniform1i(getUniformLocation(name), value) );
		}

		void ShaderOpenGL::setUniform1f(const std::string& name, float value) {
			MAR_CORE_GL_FUNC( glUniform1f(getUniformLocation(name), value) );
		}

		void ShaderOpenGL::setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
			MAR_CORE_GL_FUNC( glUniform4f(getUniformLocation(name), red, green, blue, alpha) );
		}

		void ShaderOpenGL::setUniform4fv(const std::string& name, const float* floats4) {
			MAR_CORE_GL_FUNC( glUniform4fv(getUniformLocation(name), 1, floats4) );
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

			GRAPHICS_TRACE("SHADER_OPENGL: Shader loaded successfully from source file!");

			return { vector[0], vector[1] };
		}

		uint32_t ShaderOpenGL::compileShader(uint32_t type, const std::string& sourceCode) {
			uint32_t id = glCreateShader(type);
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

				GRAPHICS_ERROR(s);

				glDeleteShader(id);
				return 0;
			}

			GRAPHICS_TRACE("SHADER_OPENGL: Shader compiled successfully!");

			return id;
		}

		uint32_t ShaderOpenGL::createShader() {
			uint32_t shaderProgramId = glCreateProgram();
			uint32_t vs = compileShader(GL_VERTEX_SHADER, m_programSource._vertexSource);
			uint32_t fs = compileShader(GL_FRAGMENT_SHADER, m_programSource._fragmentSource);

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

				GRAPHICS_ERROR(s);
				return 0;
			}

			glUseProgram(shaderProgramId);

			glDeleteShader(vs);
			glDeleteShader(fs);

			GRAPHICS_TRACE("SHADER_OPENGL: Shader created successfully!");

			return shaderProgramId;
		}


} }