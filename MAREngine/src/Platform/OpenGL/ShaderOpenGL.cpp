/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "ShaderOpenGL.h"


namespace mar {
	namespace platforms {


		void ShaderOpenGL::initialize(ShaderType shadertype) {
			if (m_initialized) {
				PLATFORM_TRACE("SHADER_OPENGL: Cannot re-initialize once compiled shader! LoadedShader - {}", m_shaderPath);
				return;
			}

			switch(shadertype) {
			case ShaderType::ENTITY_COLOR: m_shaderPath = "resources/shaders/entityColor.shader.glsl";
				break;
			case ShaderType::ENTITY_TEXTURE2D: m_shaderPath = "resources/shaders/entityTexture2D.shader.glsl";
				break;
			case ShaderType::ENTITY_CUBEMAP: m_shaderPath = "resources/shaders/entityCubemap.shader.glsl";
				break;
			case ShaderType::ENTITY_BATCHER: m_shaderPath = "resources/shaders/batcher.shader.glsl";
				break;
			default:
				PLATFORM_ERROR("SHADER_OPENGL: Cannot find selected shader!");
				return;
			}

			PLATFORM_TRACE("SHADER_OPENGL: Going to load shader from {}", m_shaderPath);

			m_id = 0;
			m_programSource = parseShader();
			m_id = createShader();
			m_initialized = true;
		}

		void ShaderOpenGL::shutdown() {
			PLATFORM_TRACE("SHADER_OPENGL: Deleting shader {} - {}", m_id, m_shaderPath);

			PLATFORM_GL_FUNC( glDeleteProgram(m_id) );
		}

		void ShaderOpenGL::bind() const {
			PLATFORM_GL_FUNC( glUseProgram(m_id) );

			PLATFORM_TRACE("SHADER_OPENGL: Binding shader {} - {}", m_id, m_shaderPath);
		}

		void ShaderOpenGL::unbind() const {
			PLATFORM_GL_FUNC( glUseProgram(0) );

			PLATFORM_TRACE("SHADER_OPENGL: Unbind shader");
		}

		void ShaderOpenGL::setUniformInt(const std::string& name, const std::vector<int32_t>& ints) {
			PLATFORM_GL_FUNC( glUniform1iv(getUniformLocation(name), ints.size(), ints.data()) );
		}

		void ShaderOpenGL::setUniformFloat(const std::string& name, const std::vector<float>& floats) {
			PLATFORM_GL_FUNC( glUniform1fv(getUniformLocation(name), floats.size(), floats.data()) );
		}

		void ShaderOpenGL::setUniformSampler(const std::string& name, int32_t sampler) {
			PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), sampler) );

			PLATFORM_TRACE("SHADER_OPENGL: setting sampler {} with int {}", name, sampler);
		}

		void ShaderOpenGL::setUniformSampler(const std::string& name, const std::vector<int32_t>& sampler) {
			PLATFORM_GL_FUNC(glUniform1iv(getUniformLocation(name), sampler.size(), sampler.data()));
		}

		void ShaderOpenGL::setUniformVectorVec3(const std::string& name, const std::vector<maths::vec3>& vec) {
			PLATFORM_GL_FUNC( glUniform3fv(getUniformLocation(name), vec.size(),  maths::vec3::value_ptr(vec)) );
		}

		void ShaderOpenGL::setUniformVectorMat4(const std::string& name, const std::vector<maths::mat4>& matrices) {
			PLATFORM_GL_FUNC( glUniformMatrix4fv(getUniformLocation(name), matrices.size(), GL_FALSE, maths::mat4::value_ptr(matrices)) );
		}

		void ShaderOpenGL::setUniformMat4f(const std::string& name, const maths::mat4& matrix4x4) {
			PLATFORM_GL_FUNC( glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, maths::mat4::value_ptr(matrix4x4)) );
		}

		void ShaderOpenGL::setUniformVector3(const std::string& name, const maths::vec3& vector3) {
			PLATFORM_GL_FUNC( glUniform3fv(getUniformLocation(name), 1, maths::vec3::value_ptr(vector3)) );
		}

		void ShaderOpenGL::setUniform1i(const std::string& name, int32_t value) {
			PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), value) );
		}

		void ShaderOpenGL::setUniform1f(const std::string& name, float value) {
			PLATFORM_GL_FUNC( glUniform1f(getUniformLocation(name), value) );
		}

		void ShaderOpenGL::setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
			PLATFORM_GL_FUNC( glUniform4f(getUniformLocation(name), red, green, blue, alpha) );
		}

		void ShaderOpenGL::setUniform4fv(const std::string& name, const float* floats4) {
			PLATFORM_GL_FUNC( glUniform4fv(getUniformLocation(name), 1, floats4) );
		}

		int ShaderOpenGL::getUniformLocation(const std::string& name) {
			if (m_uniformLocation.find(name) != m_uniformLocation.end())
				return m_uniformLocation[name];

			int location = glGetUniformLocation(m_id, name.c_str());
			if (location == -1)
				PLATFORM_ERROR("SHADER_OPENGL: Uniform {} does not exist!", name);

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

			PLATFORM_TRACE("SHADER_OPENGL: Loaded source file successfully from {}!", m_shaderPath);

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

				PLATFORM_ERROR("SHADER_OPENGL: Failed to compile shader: {} - {} - {}", m_shaderPath, (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);

				glDeleteShader(id);
				return 0;
			}

			PLATFORM_TRACE("SHADER_OPENGL: {} - {} - {} compiled successfully!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), id, m_shaderPath);

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

				PLATFORM_ERROR("SHADER_OPENGL: Failed to load shader: {} - {}", m_shaderPath, message);

				return 0;
			}

			glUseProgram(shaderProgramId);

			glDeleteShader(vs);
			glDeleteShader(fs);

			PLATFORM_TRACE("SHADER_OPENGL: {} - {} created successfully!", m_shaderPath, shaderProgramId);

			return shaderProgramId;
		}


} }