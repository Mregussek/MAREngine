/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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
			bool m_initialized{false};

			uint32_t m_id;
			std::string m_shaderPath;
			std::unordered_map<std::string, int> m_uniformLocation;
			ShaderProgramSource m_programSource;

		public:
			ShaderOpenGL() = default;

			void initialize();
			void shutdown();

			void bind() const;
			void unbind() const;

			// --- SET METHODS --- //
			void setUniformInt(const std::string& name, const std::vector<int32_t>& ints);
			void setUniformFloat(const std::string& name, const std::vector<float>& floats);
			void setUniformSampler(const std::string& name, int32_t sampler);
			void setUniformSampler(const std::string& name, const std::vector<int32_t>& sampler);
			void setUniformVectorVec3(const std::string& name, const std::vector<maths::vec3>& vec);
			void setUniformVectorMat4(const std::string& name, const std::vector<maths::mat4>& matrices);
			void setUniformMat4f(const std::string& name, const maths::mat4& matrix4x4);
			void setUniformVector3(const std::string& name, const maths::vec3& vector3);
			void setUniform1i(const std::string& name, int32_t value);
			void setUniform1f(const std::string& name, float value);
			void setUniform4f(const std::string& name, float red, float green, float blue, float alpha);
			void setUniform4fv(const std::string& name, const float* floats4);

		private:
			int getUniformLocation(const std::string& name);

			ShaderProgramSource parseShader();
			uint32_t compileShader(uint32_t type, const std::string& sourceCode);
			uint32_t createShader();
		};
    
    
} }


#endif // !MAR_ENGINE_SHADER_OPENGL_H