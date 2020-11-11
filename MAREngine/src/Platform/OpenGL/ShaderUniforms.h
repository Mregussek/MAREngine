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
		constexpr UniformItem(const char* n, uint64_t o, uint64_t m) :
			name(n),
			offset(o),
			memory(m)
		{}

		const char* name;
		uint64_t offset;
		uint64_t memory;
	};

	struct UniformBlock {
		constexpr UniformBlock(const char* n, size_t bp, uint64_t m) :
			name(n),
			bindingPoint(bp),
			memory(m)
		{}

		const char* name;
		size_t bindingPoint;
		uint64_t memory;
	};

	struct Material {
		constexpr Material(
			const char* lp,
			const char* a,
			const char* d,
			const char* s,
			const char* c,
			const char* l,
			const char* q,
			const char* shi
		) :
			lightPos(lp),
			ambient(a),
			diffuse(d),
			specular(s),
			constant(c),
			linear(l),
			quadratic(q),
			shininess(shi)
		{}


		const char* lightPos;
		const char* ambient;
		const char* diffuse;
		const char* specular;

		const char* constant;
		const char* linear;
		const char* quadratic;
		const char* shininess;
	};

	constexpr UniformItem ut_u_Model{ "CameraUniforms.u_Model", 0, sizeof(maths::mat4) };
	constexpr UniformItem ut_u_MVP{ "CameraUniforms.u_MVP", ut_u_Model.memory, sizeof(maths::mat4) };
	constexpr UniformItem ut_u_CameraPos{ "CameraUniforms.u_CameraPos", ut_u_Model.memory + ut_u_MVP.memory, sizeof(maths::vec4) }; // vec3 has a base alignment of 4N.
	constexpr UniformBlock ub_Camera{ "Camera", 0, ut_u_Model.memory * ut_u_MVP.memory * ut_u_CameraPos.memory };

	constexpr UniformItem ut_u_SeparateTransform{ "components.u_SeparateTransform", 0, 32 * sizeof(maths::mat4) };
	constexpr UniformItem ut_u_samplerTypes{ "components.u_samplerTypes", 32 * sizeof(maths::mat4), 32 * sizeof(float) };
	constexpr UniformBlock ub_EntityCmp{ "EntityCmp", 1,  ut_u_SeparateTransform.memory + ut_u_samplerTypes.memory };

	constexpr UniformItem ut_u_material{ "materialInfo.u_material", 0, 4 * sizeof(maths::vec4) + 4 * sizeof(float) };
	constexpr UniformBlock ub_MaterialInfo{ "MaterialInfo", 2, ut_u_material.memory };

	constexpr UniformItem ut_u_materialSize{ "info.u_materialSize", 0, sizeof(int32_t) };
	constexpr UniformBlock ub_MeshInfo{ "MeshInfo", 3, ut_u_materialSize.memory };

	constexpr UniformItem ut_u_Color{ "", 0, 32 * sizeof(maths::vec4) };
	constexpr UniformBlock ub_TextureSamplers{ "TextureSamplers", 4, ut_u_Color.memory };

	constexpr std::array<const char*, 32> u_SamplersColor = {
		"samplers.u_Color[0]",
		"samplers.u_Color[1]",
		"samplers.u_Color[2]",
		"samplers.u_Color[3]",
		"samplers.u_Color[4]",
		"samplers.u_Color[5]",
		"samplers.u_Color[6]",
		"samplers.u_Color[7]",
		"samplers.u_Color[8]",
		"samplers.u_Color[9]",
		"samplers.u_Color[10]",
		"samplers.u_Color[11]",
		"samplers.u_Color[12]",
		"samplers.u_Color[13]",
		"samplers.u_Color[14]",
		"samplers.u_Color[15]",
		"samplers.u_Color[16]",
		"samplers.u_Color[17]",
		"samplers.u_Color[18]",
		"samplers.u_Color[19]",
		"samplers.u_Color[20]",
		"samplers.u_Color[21]",
		"samplers.u_Color[22]",
		"samplers.u_Color[23]",
		"samplers.u_Color[24]",
		"samplers.u_Color[25]",
		"samplers.u_Color[26]",
		"samplers.u_Color[27]",
		"samplers.u_Color[28]",
		"samplers.u_Color[29]",
		"samplers.u_Color[30]",
		"samplers.u_Color[31]",
	};
	
	constexpr std::array<const char*, 32> u_Samplers2D = {
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
	
	constexpr std::array<const char*, 32> u_SamplersCube = {
		"u_SamplersCube[0]",
		"u_SamplersCube[1]",
		"u_SamplersCube[2]",
		"u_SamplersCube[3]",
		"u_SamplersCube[4]",
		"u_SamplersCube[5]",
		"u_SamplersCube[6]",
		"u_SamplersCube[7]",
		"u_SamplersCube[8]",
		"u_SamplersCube[9]",
		"u_SamplersCube[10]",
		"u_SamplersCube[11]",
		"u_SamplersCube[12]",
		"u_SamplersCube[13]",
		"u_SamplersCube[14]",
		"u_SamplersCube[15]",
		"u_SamplersCube[16]",
		"u_SamplersCube[17]",
		"u_SamplersCube[18]",
		"u_SamplersCube[19]",
		"u_SamplersCube[20]",
		"u_SamplersCube[21]",
		"u_SamplersCube[22]",
		"u_SamplersCube[23]",
		"u_SamplersCube[24]",
		"u_SamplersCube[25]",
		"u_SamplersCube[26]",
		"u_SamplersCube[27]",
		"u_SamplersCube[28]",
		"u_SamplersCube[29]",
		"u_SamplersCube[30]",
		"u_SamplersCube[31]"
	};

	constexpr std::array<Material, 32> u_material = {
		Material{
			"materialInfo.u_material[0].lightPos",
			"materialInfo.u_material[0].ambient" ,
			"materialInfo.u_material[0].diffuse" ,
			"materialInfo.u_material[0].specular" ,
			"materialInfo.u_material[0].constant" ,
			"materialInfo.u_material[0].linear" ,
			"materialInfo.u_material[0].quadratic",
			"materialInfo.u_material[0].shininess"
		},
		Material{
			"materialInfo.u_material[1].lightPos",
			"materialInfo.u_material[1].ambient" ,
			"materialInfo.u_material[1].diffuse" ,
			"materialInfo.u_material[1].specular" ,
			"materialInfo.u_material[1].constant" ,
			"materialInfo.u_material[1].linear" ,
			"materialInfo.u_material[1].quadratic" ,
			"materialInfo.u_material[1].shininess"
		},
		Material{
			"materialInfo.u_material[2].lightPos",
			"materialInfo.u_material[2].ambient" ,
			"materialInfo.u_material[2].diffuse" ,
			"materialInfo.u_material[2].specular" ,
			"materialInfo.u_material[2].constant" ,
			"materialInfo.u_material[2].linear" ,
			"materialInfo.u_material[2].quadratic" ,
			"materialInfo.u_material[2].shininess"
		},
		Material{
			"materialInfo.u_material[3].lightPos",
			"materialInfo.u_material[3].ambient" ,
			"materialInfo.u_material[3].diffuse" ,
			"materialInfo.u_material[3].specular" ,
			"materialInfo.u_material[3].constant" ,
			"materialInfo.u_material[3].linear" ,
			"materialInfo.u_material[3].quadratic" ,
			"materialInfo.u_material[3].shininess"
		},
		Material{
			"materialInfo.u_material[4].lightPos",
			"materialInfo.u_material[4].ambient" ,
			"materialInfo.u_material[4].diffuse" ,
			"materialInfo.u_material[4].specular" ,
			"materialInfo.u_material[4].constant" ,
			"materialInfo.u_material[4].linear" ,
			"materialInfo.u_material[4].quadratic" ,
			"materialInfo.u_material[4].shininess"
		},
		Material{
			"materialInfo.u_material[5].lightPos",
			"materialInfo.u_material[5].ambient" ,
			"materialInfo.u_material[5].diffuse" ,
			"materialInfo.u_material[5].specular" ,
			"materialInfo.u_material[5].constant" ,
			"materialInfo.u_material[5].linear" ,
			"materialInfo.u_material[5].quadratic" ,
			"materialInfo.u_material[5].shininess"
		},
		Material{
			"materialInfo.u_material[6].lightPos",
			"materialInfo.u_material[6].ambient" ,
			"materialInfo.u_material[6].diffuse" ,
			"materialInfo.u_material[6].specular" ,
			"materialInfo.u_material[6].constant" ,
			"materialInfo.u_material[6].linear" ,
			"materialInfo.u_material[6].quadratic" ,
			"materialInfo.u_material[6].shininess"
		},
		Material{
			"materialInfo.u_material[7].lightPos",
			"materialInfo.u_material[7].ambient" ,
			"materialInfo.u_material[7].diffuse" ,
			"materialInfo.u_material[7].specular" ,
			"materialInfo.u_material[7].constant" ,
			"materialInfo.u_material[7].linear" ,
			"materialInfo.u_material[7].quadratic" ,
			"materialInfo.u_material[7].shininess"
		},
		Material{
			"materialInfo.u_material[8].lightPos",
			"materialInfo.u_material[8].ambient" ,
			"materialInfo.u_material[8].diffuse" ,
			"materialInfo.u_material[8].specular" ,
			"materialInfo.u_material[8].constant" ,
			"materialInfo.u_material[8].linear" ,
			"materialInfo.u_material[8].quadratic" ,
			"materialInfo.u_material[8].shininess"
		},
		Material{
			"materialInfo.u_material[9].lightPos",
			"materialInfo.u_material[9].ambient" ,
			"materialInfo.u_material[9].diffuse" ,
			"materialInfo.u_material[9].specular" ,
			"materialInfo.u_material[9].constant" ,
			"materialInfo.u_material[9].linear" ,
			"materialInfo.u_material[9].quadratic" ,
			"materialInfo.u_material[9].shininess"
		},
		Material{
			"materialInfo.u_material[10].lightPos",
			"materialInfo.u_material[10].ambient" ,
			"materialInfo.u_material[10].diffuse" ,
			"materialInfo.u_material[10].specular" ,
			"materialInfo.u_material[10].constant" ,
			"materialInfo.u_material[10].linear" ,
			"materialInfo.u_material[10].quadratic" ,
			"materialInfo.u_material[10].shininess"
		},
		Material{
			"materialInfo.u_material[11].lightPos",
			"materialInfo.u_material[11].ambient" ,
			"materialInfo.u_material[11].diffuse" ,
			"materialInfo.u_material[11].specular" ,
			"materialInfo.u_material[11].constant" ,
			"materialInfo.u_material[11].linear" ,
			"materialInfo.u_material[11].quadratic" ,
			"materialInfo.u_material[11].shininess"
		},
		Material{
			"materialInfo.u_material[12].lightPos",
			"materialInfo.u_material[12].ambient" ,
			"materialInfo.u_material[12].diffuse" ,
			"materialInfo.u_material[12].specular" ,
			"materialInfo.u_material[12].constant" ,
			"materialInfo.u_material[12].linear" ,
			"materialInfo.u_material[12].quadratic" ,
			"materialInfo.u_material[12].shininess"
		},
		Material{
			"materialInfo.u_material[13].lightPos",
			"materialInfo.u_material[13].ambient" ,
			"materialInfo.u_material[13].diffuse" ,
			"materialInfo.u_material[13].specular" ,
			"materialInfo.u_material[13].constant" ,
			"materialInfo.u_material[13].linear" ,
			"materialInfo.u_material[13].quadratic" ,
			"materialInfo.u_material[13].shininess"
		},
		Material{
			"materialInfo.u_material[14].lightPos",
			"materialInfo.u_material[14].ambient" ,
			"materialInfo.u_material[14].diffuse" ,
			"materialInfo.u_material[14].specular" ,
			"materialInfo.u_material[14].constant" ,
			"materialInfo.u_material[14].linear" ,
			"materialInfo.u_material[14].quadratic" ,
			"materialInfo.u_material[14].shininess"
		},
		Material{
			"materialInfo.u_material[15].lightPos",
			"materialInfo.u_material[15].ambient" ,
			"materialInfo.u_material[15].diffuse" ,
			"materialInfo.u_material[15].specular" ,
			"materialInfo.u_material[15].constant" ,
			"materialInfo.u_material[15].linear" ,
			"materialInfo.u_material[15].quadratic" ,
			"materialInfo.u_material[15].shininess"
		},
		Material{
			"materialInfo.u_material[16].lightPos",
			"materialInfo.u_material[16].ambient" ,
			"materialInfo.u_material[16].diffuse" ,
			"materialInfo.u_material[16].specular" ,
			"materialInfo.u_material[16].constant" ,
			"materialInfo.u_material[16].linear" ,
			"materialInfo.u_material[16].quadratic" ,
			"materialInfo.u_material[16].shininess"
		},
		Material{
			"materialInfo.u_material[17].lightPos",
			"materialInfo.u_material[17].ambient" ,
			"materialInfo.u_material[17].diffuse" ,
			"materialInfo.u_material[17].specular" ,
			"materialInfo.u_material[17].constant" ,
			"materialInfo.u_material[17].linear" ,
			"materialInfo.u_material[17].quadratic" ,
			"materialInfo.u_material[17].shininess"
		},
		Material{
			"materialInfo.u_material[18].lightPos",
			"materialInfo.u_material[18].ambient" ,
			"materialInfo.u_material[18].diffuse" ,
			"materialInfo.u_material[18].specular" ,
			"materialInfo.u_material[18].constant" ,
			"materialInfo.u_material[18].linear" ,
			"materialInfo.u_material[18].quadratic" ,
			"materialInfo.u_material[18].shininess"
		},
		Material{
			"materialInfo.u_material[19].lightPos",
			"materialInfo.u_material[19].ambient" ,
			"materialInfo.u_material[19].diffuse" ,
			"materialInfo.u_material[19].specular" ,
			"materialInfo.u_material[19].constant" ,
			"materialInfo.u_material[19].linear" ,
			"materialInfo.u_material[19].quadratic" ,
			"materialInfo.u_material[19].shininess"
		},
		Material{
			"materialInfo.u_material[20].lightPos",
			"materialInfo.u_material[20].ambient" ,
			"materialInfo.u_material[20].diffuse" ,
			"materialInfo.u_material[20].specular" ,
			"materialInfo.u_material[20].constant" ,
			"materialInfo.u_material[20].linear" ,
			"materialInfo.u_material[20].quadratic" ,
			"materialInfo.u_material[20].shininess"
		},
		Material{
			"materialInfo.u_material[21].lightPos",
			"materialInfo.u_material[21].ambient" ,
			"materialInfo.u_material[21].diffuse" ,
			"materialInfo.u_material[21].specular" ,
			"materialInfo.u_material[21].constant" ,
			"materialInfo.u_material[21].linear" ,
			"materialInfo.u_material[21].quadratic" ,
			"materialInfo.u_material[21].shininess"
		},
		Material{
			"materialInfo.u_material[22].lightPos",
			"materialInfo.u_material[22].ambient" ,
			"materialInfo.u_material[22].diffuse" ,
			"materialInfo.u_material[22].specular" ,
			"materialInfo.u_material[22].constant" ,
			"materialInfo.u_material[22].linear" ,
			"materialInfo.u_material[22].quadratic" ,
			"materialInfo.u_material[22].shininess"
		},
		Material{
			"materialInfo.u_material[23].lightPos",
			"materialInfo.u_material[23].ambient" ,
			"materialInfo.u_material[23].diffuse" ,
			"materialInfo.u_material[23].specular" ,
			"materialInfo.u_material[23].constant" ,
			"materialInfo.u_material[23].linear" ,
			"materialInfo.u_material[23].quadratic" ,
			"materialInfo.u_material[23].shininess"
		},
		Material{
			"materialInfo.u_material[24].lightPos",
			"materialInfo.u_material[24].ambient" ,
			"materialInfo.u_material[24].diffuse" ,
			"materialInfo.u_material[24].specular" ,
			"materialInfo.u_material[24].constant" ,
			"materialInfo.u_material[24].linear" ,
			"materialInfo.u_material[24].quadratic" ,
			"materialInfo.u_material[24].shininess"
		},
		Material{
			"materialInfo.u_material[25].lightPos",
			"materialInfo.u_material[25].ambient" ,
			"materialInfo.u_material[25].diffuse" ,
			"materialInfo.u_material[25].specular" ,
			"materialInfo.u_material[25].constant" ,
			"materialInfo.u_material[25].linear" ,
			"materialInfo.u_material[25].quadratic" ,
			"materialInfo.u_material[25].shininess"
		},
		Material{
			"materialInfo.u_material[26].lightPos",
			"materialInfo.u_material[26].ambient" ,
			"materialInfo.u_material[26].diffuse" ,
			"materialInfo.u_material[26].specular" ,
			"materialInfo.u_material[26].constant" ,
			"materialInfo.u_material[26].linear" ,
			"materialInfo.u_material[26].quadratic" ,
			"materialInfo.u_material[26].shininess"
		},
		Material{
			"materialInfo.u_material[27].lightPos",
			"materialInfo.u_material[27].ambient" ,
			"materialInfo.u_material[27].diffuse" ,
			"materialInfo.u_material[27].specular" ,
			"materialInfo.u_material[27].constant" ,
			"materialInfo.u_material[27].linear" ,
			"materialInfo.u_material[27].quadratic" ,
			"materialInfo.u_material[27].shininess"
		},
		Material{
			"materialInfo.u_material[28].lightPos",
			"materialInfo.u_material[28].ambient" ,
			"materialInfo.u_material[28].diffuse" ,
			"materialInfo.u_material[28].specular" ,
			"materialInfo.u_material[28].constant" ,
			"materialInfo.u_material[28].linear" ,
			"materialInfo.u_material[28].quadratic" ,
			"materialInfo.u_material[28].shininess"
		},
		Material{
			"materialInfo.u_material[29].lightPos",
			"materialInfo.u_material[29].ambient" ,
			"materialInfo.u_material[29].diffuse" ,
			"materialInfo.u_material[29].specular" ,
			"materialInfo.u_material[29].constant" ,
			"materialInfo.u_material[29].linear" ,
			"materialInfo.u_material[29].quadratic" ,
			"materialInfo.u_material[29].shininess"
		},
		Material{
			"materialInfo.u_material[30].lightPos",
			"materialInfo.u_material[30].ambient" ,
			"materialInfo.u_material[30].diffuse" ,
			"materialInfo.u_material[30].specular" ,
			"materialInfo.u_material[30].constant" ,
			"materialInfo.u_material[30].linear" ,
			"materialInfo.u_material[30].quadratic" ,
			"materialInfo.u_material[30].shininess"
		},
		Material{
			"materialInfo.u_material[31].lightPos",
			"materialInfo.u_material[31].ambient" ,
			"materialInfo.u_material[31].diffuse" ,
			"materialInfo.u_material[31].specular" ,
			"materialInfo.u_material[31].constant" ,
			"materialInfo.u_material[31].linear" ,
			"materialInfo.u_material[31].quadratic" ,
			"materialInfo.u_material[31].shininess"
		} 
	};


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
