/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SHADER_H
#define MAR_ENGINE_SHADER_H

#include "../../../../mar.h"

namespace mar {
	namespace graphics {


		struct ShaderProgramSource {
			std::string _vertexSource;
			std::string _fragmentSource;
		};

		enum class ShaderType {
			ENTITY_COLOR,
			ENTITY_TEXTURE2D,
			ENTITY_CUBEMAP
		};

		template<typename SHADER>
		class Shader {
			SHADER m_shader;

		public:
			void initialize(ShaderType shadertype) { 
				m_shader.initialize(shadertype);
			}
			void shutdown() { 
				m_shader.shutdown();
			}

			void bind() const { 
				m_shader.bind();
			}
			void unbind() const { 
				m_shader.unbind();
			}

			void setUniformSampler(const std::string& name, const std::vector<int>& sampler) { 
				m_shader.setUniformSampler(name, sampler);
			}
			void setUniformVectorVec3(const std::string& name, const std::vector<maths::vec3>& vec) {
				m_shader.setUniformVectorVec3(name, vec);
			}
			void setUniformVectorMat4(const std::string& name, const std::vector<maths::mat4>& sampler) { 
				m_shader.setUniformVectorMat4(name, sampler);
			}
			void setUniformMat4f(const std::string& name, const maths::mat4& matrix4x4) {
				m_shader.setUniformMat4f(name, matrix4x4);
			}
			void setUniformVector3(const std::string& name, const maths::vec3& vector3) {
				m_shader.setUniformVector3(name, vector3);
			}
			void setUniform1i(const std::string& name, int value) {
				m_shader.setUniform1i(name, value);
			}
			void setUniform1f(const std::string& name, float value) {
				m_shader.setUniform1f(name, value);
			}
			void setUniform4f(const std::string& name, float red, float green, float blue, float alpha) {
				m_shader.setUniform4f(name, red, green, blue, alpha);
			}
			void setUniform4fv(const std::string& name, const float* floats4) {
				m_shader.setUniform4fv(name, floats4);
			}

		private:
			int getUniformLocation(const std::string& name) { 
				return m_shader.getUniformLocation(name);
			}
			ShaderProgramSource parseShader() { 
				return m_shader.parseShader();
			}
			uint32_t compileShader(uint32_t type, const std::string& source) { 
				return m_shader.compileShader();
			}
			uint32_t createShader() { 
				return m_shader.createShader();
			}
		};


} }


#define SHADER_ENTITY_COLOR ::mar::graphics::ShaderType::ENTITY_COLOR
#define SHADER_ENTITY_TEXTURE2D ::mar::graphics::ShaderType::ENTITY_TEXTURE2D
#define SHADER_ENTITY_CUBEMAP ::mar::graphics::ShaderType::ENTITY_CUBEMAP

#endif // MAR_ENGINE_SHADER_H
