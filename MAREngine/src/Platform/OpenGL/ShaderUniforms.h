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

	struct UniformBuffer {
		constexpr UniformBuffer(const char* n, size_t bp, uint32_t m) :
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
	constexpr UniformItem ut_u_CameraPos{ "CameraUniforms.u_CameraPos", ut_u_Model.memory + ut_u_MVP.memory, sizeof(maths::vec3) };
	constexpr UniformBuffer ub_Camera{ "Camera", 0, ut_u_Model.memory + ut_u_MVP.memory + ut_u_CameraPos.memory };

	// ------------ EntityCmp Uniform Block

	constexpr UniformItem ut_u_SeparateTransform{ "components.u_SeparateTransform", 0, 32 * sizeof(maths::mat4) };
	constexpr UniformItem ut_u_samplerTypes{ "components.u_samplerTypes", ut_u_SeparateTransform.memory, 32 * sizeof(float) };
	constexpr UniformBuffer ub_EntityCmp{ "EntityCmp", 1,  ut_u_SeparateTransform.memory + ut_u_samplerTypes.memory };

	// ------------ Material Uniform Block

	constexpr UniformItem ut_u_material{ "material.u_material", 0,  32 * (4 * sizeof(maths::vec4) + 4 * sizeof(float)) };
	constexpr UniformItem ut_u_lightSize{ "material.u_lightSize", ut_u_material.memory, sizeof(int32_t) };
	constexpr UniformBuffer ub_Material{ "Material", 2, ut_u_material.memory + ut_u_lightSize.memory };

	// -------------- TextureSamplers Uniform Block

	constexpr UniformItem ut_u_Color{ "samplers.u_Color", 0, 32 * sizeof(maths::vec4) };
	constexpr UniformBuffer ub_TextureSamplers{ "TextureSamplers", 3, ut_u_Color.memory };

	// -------------- u_2D Uniform Block

	constexpr UniformBuffer ub_u_2D{ "u_2D", 4, 32 * sizeof(int32_t) };

	constexpr std::array<const char*, 32> u_2D = {
		"u_2D[0]",
		"u_2D[1]",
		"u_2D[2]",
		"u_2D[3]",
		"u_2D[4]",
		"u_2D[5]",
		"u_2D[6]",
		"u_2D[7]",
		"u_2D[8]",
		"u_2D[9]",
		"u_2D[10]",
		"u_2D[11]",
		"u_2D[12]",
		"u_2D[13]",
		"u_2D[14]",
		"u_2D[15]",
		"u_2D[16]",
		"u_2D[17]",
		"u_2D[18]",
		"u_2D[19]",
		"u_2D[20]",
		"u_2D[21]",
		"u_2D[22]",
		"u_2D[23]",
		"u_2D[24]",
		"u_2D[25]",
		"u_2D[26]",
		"u_2D[27]",
		"u_2D[28]",
		"u_2D[29]",
		"u_2D[30]",
		"u_2D[31]"
	};


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
