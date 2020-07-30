/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SHADER_OPENGL_H
#define MAR_ENGINE_SHADER_OPENGL_H

#include "Shader.h"


namespace mar {
	namespace graphics {


		class ShaderOpenGL {
			bool m_initialized{false};

			uint32_t m_id;
			std::string m_shaderPath;
			std::unordered_map<std::string, int> m_uniformLocation;
			ShaderProgramSource m_programSource;

		public:
			ShaderOpenGL() = default;

			void initialize(ShaderType shadertype);
			void shutdown();

			void bind() const;
			void unbind() const;

			// --- SET METHODS --- //
			void setUniformSampler(const std::string& name, const std::vector<int>& sampler);
			void setUniformVectorVec3(const std::string& name, const std::vector<maths::vec3>& vec);
			void setUniformVectorMat4(const std::string& name, const std::vector<maths::mat4>& matrices);
			void setUniformMat4f(const std::string& name, const maths::mat4& matrix4x4);
			void setUniformVector3(const std::string& name, const maths::vec3& vector3);
			void setUniform1i(const std::string& name, int value);
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