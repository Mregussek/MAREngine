/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "ShaderOpenGL.h"
#include "../PlatformLogs.h"


namespace mar::platforms {


	// ---- PUBLIC METHODS ---- //

	void ShaderOpenGL::initialize(ShaderPaths shaderPaths) {
		if (m_initialized) {
			PLATFORM_WARN("SHADER_OPENGL: Cannot re-initialize once compiled shader! loaded Shader - {}", m_id);
			return;
		}

		PLATFORM_TRACE("SHADER_OPENGL: {} going to initialize...", m_id);

		m_shaderPaths = shaderPaths;
		
		std::string vertexSrc;
		std::string fragmentSrc;

		loadShader(vertexSrc, m_shaderPaths.vertexPath);
		loadShader(fragmentSrc, m_shaderPaths.fragmentPath);
		
		m_id = createShader(vertexSrc, fragmentSrc);

		m_initialized = true;
	}

	void ShaderOpenGL::shutdown() {
		PLATFORM_TRACE("SHADER_OPENGL: {} is being deleted...", m_id);

		PLATFORM_GL_FUNC( glDeleteProgram(m_id) );
	}

	void ShaderOpenGL::bind() const {
		PLATFORM_TRACE("SHADER_OPENGL: Binding shader {}", m_id);

		PLATFORM_GL_FUNC( glUseProgram(m_id) );
	}

	void ShaderOpenGL::unbind() const {
		PLATFORM_TRACE("SHADER_OPENGL: Unbind shader {}", m_id);

		PLATFORM_GL_FUNC( glUseProgram(0) );
	}

	void ShaderOpenGL::setUniformSampler(const char* name, int32_t sampler) const {
		PLATFORM_TRACE("SHADER_OPENGL: setting uniform sampler {} with value {} at shader {}", name, sampler, m_id);

		PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), sampler) );
	}

	void ShaderOpenGL::setupShaderUniforms(const std::array<const char*, 32>& shaderUniforms) {
		PLATFORM_TRACE("SHADER_OPENGL: initializing all uniforms at {}...", m_id);

		auto pushUniformToMap = [this](const char* uniform) {
			PLATFORM_GL_FUNC( const int32_t location = glGetUniformLocation(m_id, uniform) );
			m_uniformLocation[uniform] = location;
		};

		std::for_each(shaderUniforms.begin(), shaderUniforms.end(), pushUniformToMap);
	}

	// ---- PRIVATE METHODS ---- //

	int32_t ShaderOpenGL::getUniformLocation(const char* name) const {
		if (m_uniformLocation.find(name) != m_uniformLocation.end()) {
			return m_uniformLocation.at(name);
		}

		PLATFORM_GL_FUNC( const int32_t location = glGetUniformLocation(m_id, name) );

		if (location == -1) {
			PLATFORM_ERROR("SHADER_OPENGL: Uniform {} does not exist!", name);
		}

		return location;
	}

	void ShaderOpenGL::loadShader(std::string& buffer, const char* path) const {
		PLATFORM_TRACE("SHADER_OPENGL: loading shader from {}", path);

		FILE* file = fopen(path, "rb");
		fseek(file, 0, SEEK_END);
		const long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer.resize(length);
		size_t rc = fread(buffer.data(), 1, length, file);
		fclose(file);
	}

	uint32_t ShaderOpenGL::compileShader(uint32_t type, const std::string& sourceCode) const {
		PLATFORM_TRACE("SHADER_OPENGL: going to compile shader...");

		PLATFORM_GL_FUNC( const GLuint id = glCreateShader(type) );
		const char* src{ sourceCode.c_str() };
		PLATFORM_GL_FUNC( glShaderSource(id, 1, &src, nullptr) );
		PLATFORM_GL_FUNC( glCompileShader(id) );

		GLint compiledSucessfully{};
		PLATFORM_GL_FUNC( glGetShaderiv(id, GL_COMPILE_STATUS, &compiledSucessfully) );

		if (!compiledSucessfully) {
			GLchar message[256];
			glGetShaderInfoLog(id, sizeof(message), nullptr, message);

			PLATFORM_ERROR("SHADER_OPENGL: Failed to compile shader: {} - {}", type, message);

			PLATFORM_GL_FUNC( glDeleteShader(id) );
			return 0;
		}

		PLATFORM_INFO("SHADER_OPENGL: {} - {} compiled successfully!", type, id);

		return id;
	}

	uint32_t ShaderOpenGL::createShader(const std::string& vertSrc, const std::string& fragSrc) const {
		PLATFORM_TRACE("SHADER_OPENGL: creating shader...");

		PLATFORM_GL_FUNC( const GLuint shaderProgramId = glCreateProgram() );
		const GLuint vs = compileShader(GL_VERTEX_SHADER, vertSrc);
		const GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);

		PLATFORM_GL_FUNC( glAttachShader(shaderProgramId, vs) );
		PLATFORM_GL_FUNC( glAttachShader(shaderProgramId, fs) );

		PLATFORM_GL_FUNC( glLinkProgram(shaderProgramId) );
		PLATFORM_GL_FUNC( glValidateProgram(shaderProgramId) );

		GLint result{ 0 };
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			GLchar message[256];
			glGetProgramInfoLog(shaderProgramId, sizeof(message), nullptr, message);

			PLATFORM_ERROR("SHADER_OPENGL: Failed to load shader: \n{}\n{}", m_shaderPaths.vertexPath, m_shaderPaths.fragmentPath, message);

			return 0;
		}

		glUseProgram(shaderProgramId);

		glDeleteShader(vs);
		glDeleteShader(fs);

		PLATFORM_INFO("SHADER_OPENGL: \n{}\n{} - {} created successfully!", m_shaderPaths.vertexPath, m_shaderPaths.fragmentPath, shaderProgramId);

		return shaderProgramId;
	}


}
