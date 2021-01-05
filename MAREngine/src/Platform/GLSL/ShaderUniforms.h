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


namespace marengine {


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

	struct GLSLShaderInfo {

		// ------------- Camera Uniform Block

		inline static const UniformItem MVP{ "Camera.MVP", 0, sizeof(maths::mat4) };
		inline static const UniformBuffer CameraSSBO{ "CameraSSBO", 0, MVP.memory };

		// ------------ EntityCmp Uniform Block

		inline static const UniformItem Transform{ "Transforms.Transform", 0, 64 * sizeof(maths::mat4) };
		inline static const UniformBuffer TransformSSBO{ "TransformSSBO", 1,  Transform.memory };

		// ------------ Material Uniform Block

		inline static const UniformItem LightMaterial{ "PointLigts.LightMaterial", 0,  32 * (4 * sizeof(maths::vec4) + 4 * sizeof(float)) };
		inline static const UniformItem LightMaterialSize{ "PointLigts.LightMaterialSize", LightMaterial.memory, sizeof(int32_t) };
		inline static const UniformBuffer PointLightSSBO{ "PointLightSSBO", 2, LightMaterial.memory + LightMaterialSize.memory };

		// -------------- TextureSamplers Uniform Block

		inline static const UniformItem Colors{ "Colors.Color", 0, 32 * sizeof(maths::vec4) };
		inline static const UniformBuffer ColorsSSBO{ "ColorsSSBO", 3, Colors.memory };

		// -------------- u_2D Uniform Block

		inline static const UniformBuffer samplerTexture2D{ "samplerTexture2D", 4, 32 * sizeof(int32_t) };

		inline static const std::array<const char*, 32> samplerTexture2DArray = {
			"samplerTexture2D[0]",
			"samplerTexture2D[1]",
			"samplerTexture2D[2]",
			"samplerTexture2D[3]",
			"samplerTexture2D[4]",
			"samplerTexture2D[5]",
			"samplerTexture2D[6]",
			"samplerTexture2D[7]",
			"samplerTexture2D[8]",
			"samplerTexture2D[9]",
			"samplerTexture2D[10]",
			"samplerTexture2D[11]",
			"samplerTexture2D[12]",
			"samplerTexture2D[13]",
			"samplerTexture2D[14]",
			"samplerTexture2D[15]",
			"samplerTexture2D[16]",
			"samplerTexture2D[17]",
			"samplerTexture2D[18]",
			"samplerTexture2D[19]",
			"samplerTexture2D[20]",
			"samplerTexture2D[21]",
			"samplerTexture2D[22]",
			"samplerTexture2D[23]",
			"samplerTexture2D[24]",
			"samplerTexture2D[25]",
			"samplerTexture2D[26]",
			"samplerTexture2D[27]",
			"samplerTexture2D[28]",
			"samplerTexture2D[29]",
			"samplerTexture2D[30]",
			"samplerTexture2D[31]"
		};

		// -------------- u_2D Uniform Block

		inline static const UniformBuffer ub_u_Cubemap{ "u_Cubemap", 4, 32 * sizeof(int32_t) };

		inline static const std::array<const char*, 32> u_Cubemap = {
			"u_Cubemap[0]",
			"u_Cubemap[1]",
			"u_Cubemap[2]",
			"u_Cubemap[3]",
			"u_Cubemap[4]",
			"u_Cubemap[5]",
			"u_Cubemap[6]",
			"u_Cubemap[7]",
			"u_Cubemap[8]",
			"u_Cubemap[9]",
			"u_Cubemap[10]",
			"u_Cubemap[11]",
			"u_Cubemap[12]",
			"u_Cubemap[13]",
			"u_Cubemap[14]",
			"u_Cubemap[15]",
			"u_Cubemap[16]",
			"u_Cubemap[17]",
			"u_Cubemap[18]",
			"u_Cubemap[19]",
			"u_Cubemap[20]",
			"u_Cubemap[21]",
			"u_Cubemap[22]",
			"u_Cubemap[23]",
			"u_Cubemap[24]",
			"u_Cubemap[25]",
			"u_Cubemap[26]",
			"u_Cubemap[27]",
			"u_Cubemap[28]",
			"u_Cubemap[29]",
			"u_Cubemap[30]",
			"u_Cubemap[31]"
		};
	};


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
