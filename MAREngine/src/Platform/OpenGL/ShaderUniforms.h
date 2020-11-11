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


#ifndef MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
#define MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar::platforms::ShaderUniforms {

	struct UniformItem {
		constexpr UniformItem(const char* n, uint32_t o, uint32_t m) :
			name(n),
			offset(o),
			memory(m)
		{}

		const char* name;
		uint32_t offset;
		uint32_t memory;
	};

	struct UniformBlock {
		constexpr UniformBlock(const char* n, size_t bp, uint32_t m) :
			name(n),
			bindingPoint(bp),
			memory(m)
		{}

		const char* name;
		size_t bindingPoint;
		uint32_t memory;
	};

	// ------------- Camera Uniform Block

	constexpr UniformItem ut_u_Model{ "CameraUniforms.u_Model", 0, sizeof(maths::mat4) };
	constexpr UniformItem ut_u_MVP{ "CameraUniforms.u_MVP", ut_u_Model.memory, sizeof(maths::mat4) };
	constexpr UniformItem ut_u_CameraPos{ "CameraUniforms.u_CameraPos", ut_u_Model.memory + ut_u_MVP.memory, sizeof(maths::vec4) };
	constexpr UniformBlock ub_Camera{ "Camera", 0, ut_u_Model.memory + ut_u_MVP.memory + ut_u_CameraPos.memory };

	// ------------ EntityCmp Uniform Block

	constexpr UniformItem ut_u_SeparateTransform{ "components.u_SeparateTransform", 0, 32 * sizeof(maths::mat4) };
	constexpr UniformItem ut_u_samplerTypes{ "components.u_samplerTypes", ut_u_SeparateTransform.memory, 32 * sizeof(float) };
	constexpr UniformBlock ub_EntityCmp{ "EntityCmp", 1,  ut_u_SeparateTransform.memory + ut_u_samplerTypes.memory };

	// ------------ Material Uniform Block

	constexpr UniformItem ut_u_LightPos{ "u_material.lightPos", 0, 32 * sizeof(maths::vec4) };

	constexpr uint32_t offsetAmbient = ut_u_LightPos.memory;
	constexpr UniformItem ut_u_ambient{ "u_material.ambient", offsetAmbient, 32 * sizeof(maths::vec4) };

	constexpr uint32_t offsetDiffuse = offsetAmbient + ut_u_ambient.memory;
	constexpr UniformItem ut_u_diffuse{ "u_material.diffuse", offsetDiffuse, 32 * sizeof(maths::vec4) };

	constexpr uint32_t offsetSpecular = offsetDiffuse + ut_u_diffuse.memory;
	constexpr UniformItem ut_u_specular{ "u_material.specular", offsetSpecular, 32 * sizeof(maths::vec4) };

	constexpr uint32_t offsetConstant = offsetSpecular + ut_u_specular.memory;
	constexpr UniformItem ut_u_constant{ "u_material.constant", offsetConstant, 32 * sizeof(float) };

	constexpr uint32_t offsetLinear = offsetConstant + ut_u_constant.memory;
	constexpr UniformItem ut_u_linear{ "u_material.linear", offsetLinear, 32 * sizeof(float) };

	constexpr uint32_t offsetQuadratic = offsetLinear + ut_u_linear.memory;
	constexpr UniformItem ut_u_quadratic{ "u_material.quadratic", offsetQuadratic, 32 * sizeof(float) };

	constexpr uint32_t offsetShininess = offsetQuadratic + ut_u_quadratic.memory;
	constexpr UniformItem ut_u_shininess{ "u_material.shininess", offsetShininess, 32 * sizeof(float) };

	constexpr uint32_t offsetMaterialSize = offsetShininess + ut_u_shininess.memory;
	constexpr UniformItem ut_u_materialSize{ "u_material.materialSize", offsetMaterialSize, sizeof(int32_t) };

	constexpr UniformBlock ub_Material{ "Material", 2, offsetMaterialSize + ut_u_materialSize.memory };

	// -------------- TextureSamplers Uniform Block

	constexpr UniformItem ut_u_Color{ "", 0, 32 * sizeof(maths::vec4) };
	constexpr UniformBlock ub_TextureSamplers{ "TextureSamplers", 4, ut_u_Color.memory };


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
