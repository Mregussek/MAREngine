/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHADER_H
#define SHADER_H

#include "../../mar.h"

namespace mar {
	namespace graphics {


		struct ShaderProgramSource {
			std::string _vertexSource;
			std::string _fragmentSource;
		};

		enum class ShaderType {
			DEFAULT,
			WITHOUT_GUI,
			LIGHT
		};

		class Shader {
		public:
			virtual void initialize(ShaderType shadertype) { }
			virtual void shutdown() { }

			virtual void bind() const { }
			virtual void unbind() const { }

			virtual void setUniformSampler2D(const std::string& name, std::vector<int> sampler) { }
			virtual void setUniformVectorMat4(const std::string& name, std::vector<glm::mat4> sampler) { }
			virtual void setUniformMat4f(const std::string& name, const glm::mat4& matrix4x4) { }
			virtual void setUniformVector3(const std::string& name, const glm::vec3& vector3) { }
			virtual void setUniform1i(const std::string& name, int value) { }
			virtual void setUniform4f(const std::string& name, float red, float green, float blue, float alpha) { }
			virtual void setUniform4fv(const std::string& name, const float* floats4) { }

		private:
			virtual int getUniformLocation(const std::string& name) { return 0; }

			virtual ShaderProgramSource parseShader() { return { "empty", "empty" }; }
			virtual unsigned int compileShader(unsigned int type, const std::string& source) { return 0; }
			virtual unsigned int createShader() { return 0; }
		};


} }

#endif // SHADER_H
