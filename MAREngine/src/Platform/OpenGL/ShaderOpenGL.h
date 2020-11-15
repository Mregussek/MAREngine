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
#include "../PlatformLogs.h"
#include "ShaderBufferStorageOpenGL.h"
#include "ShaderUniforms.h"


namespace mar::graphics { struct LightMaterial; }
namespace mar::platforms {


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

		typedef ShaderUniforms::UniformBuffer UniformBuffer;
		typedef ShaderUniforms::UniformItem UniformItem;

	public:

		ShaderOpenGL() = default;

		void initialize(ShaderPaths shaderPaths);
		void shutdown();

		void bind() const;
		void unbind() const;

		ShaderBufferStorageOpenGL& createShaderBufferStorage();

		void uploadUniformFloat(const UniformBuffer& buffer, const UniformItem& item, const float& f) const;
		void uploadUniformFloat(const UniformBuffer& buffer, const UniformItem& item, const std::vector<float>& floats) const;
		void uploadUniformInt(const UniformBuffer& buffer, const UniformItem& item, const int32_t& i) const;
		void uploadUniformInt(const UniformBuffer& buffer, const UniformItem& item, const std::vector<int32_t>& ints) const;
		void uploadUniformSampler(const UniformBuffer& buffer, const UniformItem& item, const int32_t& sampler) const;
		void uploadUniformSampler(const UniformBuffer& buffer, const UniformItem& item, const std::vector<int32_t>& sampler) const;
		void uploadUniformVec3(const UniformBuffer& buffer, const UniformItem& item, const maths::vec3& vector3) const;
		void uploadUniformVec3(const UniformBuffer& buffer, const UniformItem& item, const std::vector<maths::vec3>& vec) const;
		void uploadUniformMat4(const UniformBuffer& buffer, const UniformItem& item, const maths::mat4& matrix4x4) const;
		void uploadUniformMat4(const UniformBuffer& buffer, const UniformItem& item, const std::vector<maths::mat4>& matrices) const;

		void uploadUniformLightMaterial(const UniformBuffer& buffer, const UniformItem& item, const std::vector<graphics::LightMaterial>& lights) const;

		void uploadUniformVec3AtIndex(const UniformBuffer& buffer, const UniformItem& item, int32_t index, const maths::vec3& vector3) const;
		void uploadUniformFloatAtIndex(const UniformBuffer& buffer, const UniformItem& item, int32_t index, const float& f) const;

	private:

		const ShaderBufferStorageOpenGL& getCorrectShaderBuffer(const UniformBuffer& block) const;

		void loadShader(std::string& buffer, const char* path) const;
		uint32_t compileShader(uint32_t type, const std::string& sourceCode) const;
		uint32_t createShader(const std::string& vertSrc, const std::string& fragSrc) const;

		
		std::vector<ShaderBufferStorageOpenGL> m_shaderBuffers;
		ShaderPaths m_shaderPaths;
		uint32_t m_id{ 0 };
		bool m_initialized{ false };

	};
    
    
}


#endif // !MAR_ENGINE_SHADER_OPENGL_H