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
		m_initialized = true;
	}

	void ShaderOpenGL::shutdown() {
		PLATFORM_TRACE("SHADER_OPENGL: Deleting shader {}", m_id);

		std::for_each(m_uniformBuffers.begin(), m_uniformBuffers.end(), [](ShaderBufferStorageOpenGL& ubo) {
			ubo.close();
		});

		m_uniformBuffers.clear();

		PLATFORM_GL_FUNC( glDeleteProgram(m_id) );
	}

	ShaderBufferStorageOpenGL& ShaderOpenGL::submitUniformBuffer() {
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

	void ShaderOpenGL::uploadUniformFloat(const UniformBuffer& buffer, const UniformItem& item, const float& f) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, sizeof(float), &f);
	}

	void ShaderOpenGL::uploadUniformFloat(const UniformBuffer& buffer, const UniformItem& item, const std::vector<float>& floats) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, floats.size() * sizeof(float), floats.data());
	}

	void ShaderOpenGL::uploadUniformInt(const UniformBuffer& buffer, const UniformItem& item, const int32_t& i) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<int32_t>(item.offset, sizeof(int32_t), &i);
	}

	void ShaderOpenGL::uploadUniformInt(const UniformBuffer& buffer, const UniformItem& item, const std::vector<int32_t>& ints) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<int32_t>(item.offset, ints.size() * sizeof(int32_t), ints.data());
	}

	void ShaderOpenGL::uploadUniformSampler(const UniformBuffer& buffer, const UniformItem& item, const int32_t& sampler) const {
		uploadUniformInt(buffer, item, sampler);
	}

	void ShaderOpenGL::uploadUniformSampler(const UniformBuffer& buffer, const UniformItem& item, const std::vector<int32_t>& samplers) const {
		uploadUniformInt(buffer, item, samplers);
	}

	void ShaderOpenGL::uploadUniformVec3(const UniformBuffer& buffer, const UniformItem& item, const maths::vec3& vector3) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, sizeof(maths::vec3), maths::vec3::value_ptr(vector3));
	}

	void ShaderOpenGL::uploadUniformVec3(const UniformBuffer& buffer, const UniformItem& item, const std::vector<maths::vec3>& vec) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, vec.size() * sizeof(maths::vec3), maths::vec3::value_ptr(vec));
	}

	void ShaderOpenGL::uploadUniformMat4(const UniformBuffer& buffer, const UniformItem& item, const maths::mat4& matrix4x4) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, sizeof(maths::mat4), maths::mat4::value_ptr(matrix4x4));
	}

	void ShaderOpenGL::uploadUniformMat4(const UniformBuffer& buffer, const UniformItem& item, const std::vector<maths::mat4>& matrices) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset, matrices.size() * sizeof(maths::mat4), maths::mat4::value_ptr(matrices));
	}

	void ShaderOpenGL::uploadUniformVec3AtIndex(const UniformBuffer& buffer, const UniformItem& item, int32_t index, const maths::vec3& vector3) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);

		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset + index * sizeof(maths::vec3), sizeof(maths::vec3), maths::vec3::value_ptr(vector3));
	}

	void ShaderOpenGL::uploadUniformFloatAtIndex(const UniformBuffer& buffer, const UniformItem& item, int32_t index, const float& f) const {
		const ShaderBufferStorageOpenGL& shaderBuffer = getCorrectShaderBuffer(buffer);
		
		shaderBuffer.bind();
		shaderBuffer.update<float>(item.offset + index * sizeof(float), sizeof(float), &f);
	}

	// ---- PRIVATE METHODS ---- //

	const ShaderBufferStorageOpenGL& ShaderOpenGL::getCorrectShaderBuffer(const UniformBuffer& block) const {
		return *std::find_if(m_uniformBuffers.cbegin(), m_uniformBuffers.cend(), [&block](const ShaderBufferStorageOpenGL& ubo) {
			return std::strcmp(ubo.m_uniformBuffer.name, block.name) == 0;
		});
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
