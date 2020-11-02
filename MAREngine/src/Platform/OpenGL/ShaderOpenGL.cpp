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

	void ShaderOpenGL::initialize(std::string shader /* = "resources/shaders/batcher.shader.glsl" */) {
		if (m_initialized) {
			PLATFORM_TRACE("SHADER_OPENGL: Cannot re-initialize once compiled shader! LoadedShader - {}", m_shaderPath);
			return;
		}

		m_shaderPath = shader;

		PLATFORM_TRACE("SHADER_OPENGL: Going to load shader from {}", m_shaderPath);

		m_id = 0;
		m_programSource = parseShader();
		m_id = createShader();
		setupShaderUniforms();
		m_initialized = true;
	}

	void ShaderOpenGL::shutdown() const {
		PLATFORM_TRACE("SHADER_OPENGL: Deleting shader {} - {}", m_id, m_shaderPath);

		PLATFORM_GL_FUNC( glDeleteProgram(m_id) );
	}

	void ShaderOpenGL::bind() const {
		PLATFORM_GL_FUNC( glUseProgram(m_id) );

		PLATFORM_TRACE("SHADER_OPENGL: Binding shader {} - {}", m_id, m_shaderPath);
	}

	void ShaderOpenGL::unbind() const {
		PLATFORM_GL_FUNC( glUseProgram(0) );

		PLATFORM_TRACE("SHADER_OPENGL: Unbind shader {} - {}", m_id, m_shaderPath);
	}

	void ShaderOpenGL::setUniformFloat(const char* name, float f) const {
		PLATFORM_GL_FUNC( glUniform1f(getUniformLocation(name), f) );
	}

	void ShaderOpenGL::setUniformFloat(const char* name, const std::vector<float>& floats) const {
		PLATFORM_GL_FUNC( glUniform1fv(getUniformLocation(name), floats.size(), floats.data()) );
	}

	void ShaderOpenGL::setUniformInt(const char* name, int32_t i) const {
		PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), i) );
	}

	void ShaderOpenGL::setUniformInt(const char* name, const std::vector<int32_t>& ints) const {
		PLATFORM_GL_FUNC( glUniform1iv(getUniformLocation(name), ints.size(), ints.data()) );
	}

	void ShaderOpenGL::setUniformSampler(const char* name, int32_t sampler) const {
		PLATFORM_GL_FUNC( glUniform1i(getUniformLocation(name), sampler) );
	}

	void ShaderOpenGL::setUniformSampler(const char* name, const std::vector<int32_t>& sampler) const {
		PLATFORM_GL_FUNC( glUniform1iv(getUniformLocation(name), sampler.size(), sampler.data()) );
	}

	void ShaderOpenGL::setUniformVec3(const char* name, maths::vec3 vector3) const {
		PLATFORM_GL_FUNC( glUniform3fv(getUniformLocation(name), 1, maths::vec3::value_ptr(vector3)) );
	}

	void ShaderOpenGL::setUniformVec3(const char* name, const std::vector<maths::vec3>& vec) const {
		PLATFORM_GL_FUNC(glUniform3fv(getUniformLocation(name), vec.size(), maths::vec3::value_ptr(vec)));
	}

	void ShaderOpenGL::setUniformMat4(const char* name, const maths::mat4& matrix4x4) const {
		PLATFORM_GL_FUNC(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, maths::mat4::value_ptr(matrix4x4)));
	}

	void ShaderOpenGL::setUniformMat4(const char* name, const std::vector<maths::mat4>& matrices) const {
		PLATFORM_GL_FUNC(glUniformMatrix4fv(getUniformLocation(name), matrices.size(), GL_FALSE, maths::mat4::value_ptr(matrices)));
	}

	// ---- PRIVATE METHODS ---- //

	int32_t ShaderOpenGL::getUniformLocation(const char* name) const {
		if (m_uniformLocation.find(name) != m_uniformLocation.end()) {
			return m_uniformLocation.at(name);
		}

		const int32_t location = glGetUniformLocation(m_id, name);

		if (location == -1) {
			PLATFORM_ERROR("SHADER_OPENGL: Uniform {} does not exist!", name);
		}
			
		return location;
	}

	void ShaderOpenGL::setupShaderUniforms() {
		using namespace ShaderUniforms;
		
		auto pushUniformToMap = [this](const char* uniform) {
			const int32_t location = glGetUniformLocation(m_id, uniform);
			m_uniformLocation[uniform] = location;
		};

		auto pushMaterialToMap = [pushUniformToMap, this](const Material& material) {
			pushUniformToMap(material.lightPos);
			pushUniformToMap(material.ambient);
			pushUniformToMap(material.diffuse);
			pushUniformToMap(material.specular);
			pushUniformToMap(material.constant);
			pushUniformToMap(material.linear);
			pushUniformToMap(material.quadratic);
			pushUniformToMap(material.shininess);
		};

		pushUniformToMap(u_Model);
		pushUniformToMap(u_MVP);
		pushUniformToMap(u_SeparateTransform);
		pushUniformToMap(u_samplerTypes);
		pushUniformToMap(u_materialSize);
		pushUniformToMap(u_CameraPos);

		std::for_each(u_SamplersColor.begin(), u_SamplersColor.end(), pushUniformToMap);
		std::for_each(u_Samplers2D.begin(), u_Samplers2D.end(), pushUniformToMap);
		std::for_each(u_SamplersCube.begin(), u_SamplersCube.end(), pushUniformToMap);
		std::for_each(u_material.begin(), u_material.end(), pushMaterialToMap);

		PLATFORM_TRACE("SHADER_OPENGL: initialized all uniforms!");
	}

	ShaderProgramSource ShaderOpenGL::parseShader() const {
		enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

		std::ifstream stream(m_shaderPath);
		std::string line;
		std::vector<std::string> vector(2);
		auto type = ShaderType::None;

		while (std::getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) { type = ShaderType::Vertex; }
				else if (line.find("fragment") != std::string::npos) { type = ShaderType::Fragment; }
			}
			else { vector[(int)type] += line + "\n"; }
		}

		PLATFORM_TRACE("SHADER_OPENGL: Loaded source file successfully from {}!", m_shaderPath);

		return { vector[0], vector[1] };
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

			PLATFORM_ERROR("SHADER_OPENGL: Failed to compile shader: {} - {} - {}", m_shaderPath, (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);

			PLATFORM_GL_FUNC( glDeleteShader(id) );
			return 0;
		}

		PLATFORM_TRACE("SHADER_OPENGL: {} - {} - {} compiled successfully!", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), id, m_shaderPath);

		return id;
	}

	uint32_t ShaderOpenGL::createShader() const {
		PLATFORM_GL_FUNC( const GLuint shaderProgramId = glCreateProgram() );
		const GLuint vs = compileShader(GL_VERTEX_SHADER, m_programSource.vertexSource);
		const GLuint fs = compileShader(GL_FRAGMENT_SHADER, m_programSource.fragmentSource);

		PLATFORM_GL_FUNC( glAttachShader(shaderProgramId, vs) );
		PLATFORM_GL_FUNC( glAttachShader(shaderProgramId, fs) );

		PLATFORM_GL_FUNC( glBindAttribLocation(shaderProgramId, 0, "position") );
		PLATFORM_GL_FUNC( glBindAttribLocation(shaderProgramId, 1, "lightNormal") );
		PLATFORM_GL_FUNC( glBindAttribLocation(shaderProgramId, 2, "texCoord") );
		PLATFORM_GL_FUNC( glBindAttribLocation(shaderProgramId, 3, "shapeIndex") );

		PLATFORM_GL_FUNC( glLinkProgram(shaderProgramId) );
		PLATFORM_GL_FUNC( glValidateProgram(shaderProgramId) );

		GLint result{ 0 };
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &result);
		if (result == GL_FALSE) {
			GLchar message[256];
			glGetProgramInfoLog(shaderProgramId, sizeof(message), nullptr, message);

			PLATFORM_ERROR("SHADER_OPENGL: Failed to load shader: {} - {}", m_shaderPath, message);

			return 0;
		}

		glUseProgram(shaderProgramId);

		glDeleteShader(vs);
		glDeleteShader(fs);

		PLATFORM_TRACE("SHADER_OPENGL: {} - {} created successfully!", m_shaderPath, shaderProgramId);

		return shaderProgramId;
	}


}
