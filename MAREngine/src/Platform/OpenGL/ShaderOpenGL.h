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


namespace mar {
	namespace platforms {


		struct ShaderProgramSource {
			std::string _vertexSource;
			std::string _fragmentSource;
		};


		class ShaderOpenGL {
		public:

			ShaderOpenGL() = default;

			void initialize(std::string shader = "resources/shaders/batcher.shader.glsl");
			void shutdown();

			void bind() const;
			void unbind() const;

			void setUniformFloat(const std::string& name, float f);
			void setUniformFloat(const std::string& name, const std::vector<float>& floats);
			void setUniformInt(const std::string& name, int32_t i);
			void setUniformInt(const std::string& name, const std::vector<int32_t>& ints);
			void setUniformSampler(const std::string& name, int32_t sampler);
			void setUniformSampler(const std::string& name, const std::vector<int32_t>& sampler);
			void setUniformVec3(const std::string& name, maths::vec3 vector3);
			void setUniformVec3(const std::string& name, const std::vector<maths::vec3>& vec);
			void setUniformMat4(const std::string& name, const maths::mat4& matrix4x4);
			void setUniformMat4(const std::string& name, const std::vector<maths::mat4>& matrices);

		private:

			int getUniformLocation(const std::string& name);

			ShaderProgramSource parseShader();
			uint32_t compileShader(uint32_t type, const std::string& sourceCode);
			uint32_t createShader();


			bool m_initialized{ false };

			uint32_t m_id{ 0 };
			std::string m_shaderPath{ "" };
			std::unordered_map<std::string, int> m_uniformLocation;
			ShaderProgramSource m_programSource;
		};
    
    
} }


#endif // !MAR_ENGINE_SHADER_OPENGL_H