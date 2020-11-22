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
#include "ShaderUniforms.h"
#include "../../Core/graphics/Renderer/RenderContainer.h"


namespace mar::platforms {


	// ---- PUBLIC METHODS ---- //

	void ShaderOpenGL::initialize(ShaderPaths shaderPaths) {
		if (m_initialized) {
			PLATFORM_TRACE("SHADER_OPENGL: Cannot re-initialize once compiled shader! loaded Shader - {}", m_id);
			return;
		}

		m_shaderPaths = shaderPaths;
		
		std::string vertexSrc;
		std::string fragmentSrc;

		loadShader(vertexSrc, m_shaderPaths.vertexPath);
		loadShader(fragmentSrc, m_shaderPaths.fragmentPath);
		
		m_id = createShader(vertexSrc, fragmentSrc);
		
		setupShaderUniforms();

		m_initialized = true;
	}

	void ShaderOpenGL::shutdown() {
		PLATFORM_TRACE("SHADER_OPENGL: Deleting shader {}", m_id);

		std::for_each(m_shaderBuffers.begin(), m_shaderBuffers.end(), [](ShaderBufferStorageOpenGL& ssbo) {
			ssbo.close();
		});

		std::for_each(m_uniformBuffers.begin(), m_uniformBuffers.end(), [](UniformBufferOpenGL& ubo) {
			ubo.close();
		});

		m_shaderBuffers.clear();
		m_uniformBuffers.clear();

		PLATFORM_GL_FUNC( glDeleteProgram(m_id) );
	}

	ShaderBufferStorageOpenGL& ShaderOpenGL::createShaderBufferStorage() {
		return m_shaderBuffers.emplace_back();
	}

	UniformBufferOpenGL& ShaderOpenGL::createUniformBufferObject() {
		return m_uniformBuffers.emplace_back();
	}

	void ShaderOpenGL::bind() const {
		PLATFORM_GL_FUNC( glUseProgram(m_id) );

		PLATFORM_TRACE("SHADER_OPENGL: Binding shader {}", m_id);
	}

	void ShaderOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glUseProgram(0) );

		PLATFORM_TRACE("SHADER_OPENGL: Unbind shader {}", m_id);
	}

	const ShaderBufferStorageOpenGL& ShaderOpenGL::getCorrectShaderBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_shaderBuffers.cbegin(), m_shaderBuffers.cend(), [&buffer](const ShaderBufferStorageOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	const UniformBufferOpenGL& ShaderOpenGL::getCorrectUniformBuffer(const UniformBuffer& buffer) const {
		return *std::find_if(m_uniformBuffers.cbegin(), m_uniformBuffers.cend(), [&buffer](const UniformBufferOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, buffer.name) == 0;
		});
	}

	void ShaderOpenGL::setUniformSampler(const char* name, int32_t sampler) const {
		PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), sampler) );
	}

	// ---- PRIVATE METHODS ---- //

	void ShaderOpenGL::setupShaderUniforms() {
		using namespace ShaderUniforms;

		auto pushUniformToMap = [this](const char* uniform) {
			PLATFORM_GL_FUNC( const int32_t location = glGetUniformLocation(m_id, uniform) );
			m_uniformLocation[uniform] = location;
		};

		std::for_each(u_2D.begin(), u_2D.end(), pushUniformToMap);
		//std::for_each(u_Cubemap.begin(), u_Cubemap.end(), pushUniformToMap);

		PLATFORM_TRACE("SHADER_OPENGL: initialized all uniforms!");
	}

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
		FILE* file = fopen(path, "rb");
		fseek(file, 0, SEEK_END);
		const long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer.resize(length);
		size_t rc = fread(buffer.data(), 1, length, file);
		fclose(file);
	}

	uint32_t ShaderOpenGL::compileShader(uint32_t type, const std::string& sourceCode) const {
		PLATFORM_GL_FUNC( const GLuint id = glCreateShader(type) );
		const char* src = sourceCode.c_str();
		PLATFORM_GL_FUNC( glShaderSource(id, 1, &src, nullptr) );
		PLATFORM_GL_FUNC( glCompileShader(id) );

		GLint compiledSucessfully;
		PLATFORM_GL_FUNC( glGetShaderiv(id, GL_COMPILE_STATUS, &compiledSucessfully) );

		if (!compiledSucessfully) {
			GLchar message[256];
			glGetShaderInfoLog(id, sizeof(message), nullptr, message);

			PLATFORM_ERROR("SHADER_OPENGL: Failed to compile shader: {} - {}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);

			PLATFORM_GL_FUNC( glDeleteShader(id) );
			return 0;
		}

		PLATFORM_TRACE("SHADER_OPENGL: {} - {} compiled successfully!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), id);

		return id;
	}

	uint32_t ShaderOpenGL::createShader(const std::string& vertSrc, const std::string& fragSrc) const {
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

		PLATFORM_TRACE("SHADER_OPENGL: \n{}\n{} - {} created successfully!", m_shaderPaths.vertexPath, m_shaderPaths.fragmentPath, shaderProgramId);

		return shaderProgramId;
	}


}
