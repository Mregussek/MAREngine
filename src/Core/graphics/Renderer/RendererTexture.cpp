/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Shader/ShaderUniforms.h"
#include "RendererTexture.h"

namespace mar {
	namespace graphics {


		void RendererTexture::passLightToShader(ShaderOpenGL& shader, const ecs::LightStorage& light) {
			MAR_CORE_ASSERT(light.positions.size() == light.components.size(), "Light positions are not equal to light components!");

			using namespace ShaderUniforms;

			for (size_t i = 0; i < light.components.size(); i++) {
				shader.setUniformVector3(u_material[i].lightPos, light.positions[i]);

				shader.setUniformVector3(u_material[i].ambient, light.components[i].ambient);
				shader.setUniformVector3(u_material[i].diffuse, light.components[i].diffuse);
				shader.setUniformVector3(u_material[i].specular, light.components[i].specular);

				shader.setUniform1f(u_material[i].shininess, light.components[i].shininess);

				shader.setUniform1f(u_material[i].constant, light.components[i].constant);
				shader.setUniform1f(u_material[i].linear, light.components[i].linear);
				shader.setUniform1f(u_material[i].quadratic, light.components[i].quadratic);
			}

			shader.setUniform1i(u_materialSize, light.positions.size());

			GRAPHICS_TRACE("RENDERERENTITY_COLOR: passed light to shader!");
		}


} }