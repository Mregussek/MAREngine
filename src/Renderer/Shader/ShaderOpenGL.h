/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHADER_OPENGL_H
#define SHADER_OPENGL_H

#include "Shader.h"

namespace mar {
	namespace graphics {


		class ShaderOpenGL : public Shader {
			bool m_initialized{false};

			unsigned int m_id;
			std::string m_shaderPath;
			std::unordered_map<std::string, int> m_uniformLocation;
			ShaderProgramSource m_programSource;

		public:
			ShaderOpenGL() = default;

			void initialize(ShaderType shadertype) override;
			void shutdown() override;

			void bind() const;
			void unbind() const;

			// --- SET METHODS --- //
			void setUniformSampler(const std::string& name, const std::vector<int>& sampler) override;
			void setUniformVectorVec3(const std::string& name, const std::vector<glm::vec3>& vec) override;
			void setUniformVectorMat4(const std::string& name, const std::vector<glm::mat4>& matrices) override;
			void setUniformMat4f(const std::string& name, const glm::mat4& matrix4x4) override;
			void setUniformVector3(const std::string& name, const glm::vec3& vector3) override;
			void setUniform1i(const std::string& name, int value) override;
			void setUniform1f(const std::string& name, float value) override;
			void setUniform4f(const std::string& name, float red, float green, float blue, float alpha) override;
			void setUniform4fv(const std::string& name, const float* floats4) override;

		private:
			int getUniformLocation(const std::string& name) override;

			ShaderProgramSource parseShader() override;
			unsigned int compileShader(unsigned int type, const std::string& sourceCode) override;
			unsigned int createShader() override;
		};
    
    
} }

#endif // SHADER_OPENGL_H