/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SHADER_OPENGL_H
#define SHADER_OPENGL_H

#include "Shader.h"

namespace mar {
	namespace graphics {

    
		struct {
#ifndef IMPORTED_FROM_GITHUB 
			const std::string mainPath{ "resources/shaders/basic.shader" };
			const std::string withoutGUIPath{ "resources/shaders/basicWithoutGUI.shader" };
#else
			const std::string mainPath{ "../../../resources/shaders/basic.shader" };
			const std::string withoutGUIPath{ "../../../resources/shaders/basicWithoutGUI.shader" };
#endif
		} ShaderOpenGLSettings;

		class ShaderOpenGL : public Shader {
			unsigned int _rendererId;
			std::string _filePath;
			std::unordered_map<std::string, int> _uniformLocationCache;
			ShaderProgramSource _programSource;

		public:
			ShaderOpenGL() = default;

			void initialize(ShaderType shadertype) override;
			void shutdown() override;

			void bind() const;
			void unbind() const;

			void setUniformSampler2D(const std::string& name, std::vector<int> sampler) override;
			void setUniformVectorMat4(const std::string& name, std::vector<glm::mat4> sampler) override;
			void setUniformMat4f(const std::string& name, const glm::mat4& matrix4x4) override;
			void setUniformVector3(const std::string& name, const glm::vec3& vector3) override;
			void setUniform1i(const std::string& name, int value) override;
			void setUniform1f(const std::string& name, float value) override;
			void setUniform4f(const std::string& name, float red, float green, float blue, float alpha) override;
			void setUniform4fv(const std::string& name, const float* floats4) override;

		private:
			int getUniformLocation(const std::string& name) override;

			ShaderProgramSource parseShader() override;
			unsigned int compileShader(unsigned int type, const std::string& source) override;
			unsigned int createShader() override;
		};
    
    
} }

#endif // SHADER_OPENGL_H