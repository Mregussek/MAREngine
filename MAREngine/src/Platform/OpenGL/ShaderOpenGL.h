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


#ifndef MAR_ENGINE_PLATFORMS_SHADER_OPENGL_H
#define MAR_ENGINE_PLATFORMS_SHADER_OPENGL_H


#include "../../mar.h"


namespace marengine {


	struct ShaderPaths {
		ShaderPaths() = default;

		ShaderPaths(const char* vert, const char* frag, const char* geo) :
			vertexPath(vert),
			fragmentPath(frag),
			geometryPath(geo)
		{}

		const char* vertexPath{ nullptr };
		const char* fragmentPath{ nullptr };
		const char* geometryPath{ nullptr };
	};


	class ShaderOpenGL {

	public:

		ShaderOpenGL() = default;

		void initialize(ShaderPaths shaderPaths);
		void shutdown();

		void bind() const;
		void unbind() const;

		void setupShaderUniforms(const std::array<const char*, 32>& shaderUniforms);
		void setUniformSampler(const char* name, int32_t sampler) const;

	private:

		int32_t getUniformLocation(const char* name) const;

		void loadShader(std::string& buffer, const char* path) const;
		uint32_t compileShader(uint32_t type, const std::string& sourceCode) const;
		uint32_t createShader(const std::string& vertSrc, const std::string& fragSrc) const;

		
		std::unordered_map<const char*, int32_t> m_uniformLocation;
		ShaderPaths m_shaderPaths;
		uint32_t m_id{ 0 };
		bool m_initialized{ false };

	};
    
    
}


#endif // !MAR_ENGINE_SHADER_OPENGL_H