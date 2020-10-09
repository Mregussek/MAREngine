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


	struct Material {
		std::string lightPos;
		std::string ambient;
		std::string diffuse;
		std::string specular;
	
		std::string constant;
		std::string linear;
		std::string quadratic;
		std::string shininess;
	};
	
	inline static const std::string u_materialSize{ "u_materialSize" };
	inline static const std::string u_samplerTypes{ "u_samplerTypes" };
	inline static const std::string u_separateTransform{ "u_SeparateTransform" };
	inline static const std::string u_CameraPos{ "u_CameraPos" };
	inline static const std::string u_MVP{ "u_MVP" };
	inline static const std::string u_Model{ "u_Model" };
	
	inline static const std::vector<std::string> u_SamplersColor = {
		"u_SamplersColor[0]",
		"u_SamplersColor[1]",
		"u_SamplersColor[2]",
		"u_SamplersColor[3]",
		"u_SamplersColor[4]",
		"u_SamplersColor[5]",
		"u_SamplersColor[6]",
		"u_SamplersColor[7]",
		"u_SamplersColor[8]",
		"u_SamplersColor[9]",
		"u_SamplersColor[10]",
		"u_SamplersColor[11]",
		"u_SamplersColor[12]",
		"u_SamplersColor[13]",
		"u_SamplersColor[14]",
		"u_SamplersColor[15]",
		"u_SamplersColor[16]",
		"u_SamplersColor[17]",
		"u_SamplersColor[18]",
		"u_SamplersColor[19]",
		"u_SamplersColor[20]",
		"u_SamplersColor[21]",
		"u_SamplersColor[22]",
		"u_SamplersColor[23]",
		"u_SamplersColor[24]",
		"u_SamplersColor[25]",
		"u_SamplersColor[26]",
		"u_SamplersColor[27]",
		"u_SamplersColor[28]",
		"u_SamplersColor[29]",
		"u_SamplersColor[30]",
		"u_SamplersColor[31]"
	};
	
	inline static const std::vector<std::string> u_Samplers2D = {
		"u_Samplers2D[0]",
		"u_Samplers2D[1]",
		"u_Samplers2D[2]",
		"u_Samplers2D[3]",
		"u_Samplers2D[4]",
		"u_Samplers2D[5]",
		"u_Samplers2D[6]",
		"u_Samplers2D[7]",
		"u_Samplers2D[8]",
		"u_Samplers2D[9]",
		"u_Samplers2D[10]",
		"u_Samplers2D[11]",
		"u_Samplers2D[12]",
		"u_Samplers2D[13]",
		"u_Samplers2D[14]",
		"u_Samplers2D[15]",
		"u_Samplers2D[16]",
		"u_Samplers2D[17]",
		"u_Samplers2D[18]",
		"u_Samplers2D[19]",
		"u_Samplers2D[20]",
		"u_Samplers2D[21]",
		"u_Samplers2D[22]",
		"u_Samplers2D[23]",
		"u_Samplers2D[24]",
		"u_Samplers2D[25]",
		"u_Samplers2D[26]",
		"u_Samplers2D[27]",
		"u_Samplers2D[28]",
		"u_Samplers2D[29]",
		"u_Samplers2D[30]",
		"u_Samplers2D[31]"
	};
	
	inline static const std::vector<std::string> u_SamplersCube = {
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

	inline static const std::vector<std::string> u_SeparateTransform = {
		"u_SeparateTransform[0]",
		"u_SeparateTransform[1]",
		"u_SeparateTransform[2]",
		"u_SeparateTransform[3]",
		"u_SeparateTransform[4]",
		"u_SeparateTransform[5]",
		"u_SeparateTransform[6]",
		"u_SeparateTransform[7]",
		"u_SeparateTransform[8]",
		"u_SeparateTransform[9]",
		"u_SeparateTransform[10]",
		"u_SeparateTransform[11]",
		"u_SeparateTransform[12]",
		"u_SeparateTransform[13]",
		"u_SeparateTransform[14]",
		"u_SeparateTransform[15]",
		"u_SeparateTransform[16]",
		"u_SeparateTransform[17]",
		"u_SeparateTransform[18]",
		"u_SeparateTransform[19]",
		"u_SeparateTransform[20]",
		"u_SeparateTransform[21]",
		"u_SeparateTransform[22]",
		"u_SeparateTransform[23]",
		"u_SeparateTransform[24]",
		"u_SeparateTransform[25]",
		"u_SeparateTransform[26]",
		"u_SeparateTransform[27]",
		"u_SeparateTransform[28]",
		"u_SeparateTransform[29]",
		"u_SeparateTransform[30]",
		"u_SeparateTransform[31]"
	};

	inline static const std::vector<Material> u_material = {
		{
			"u_material[0].lightPos",
			"u_material[0].ambient" ,
			"u_material[0].diffuse" ,
			"u_material[0].specular" ,
			"u_material[0].constant" ,
			"u_material[0].linear" ,
			"u_material[0].quadratic" ,
			"u_material[0].shininess"
		},
		{
			"u_material[1].lightPos",
			"u_material[1].ambient" ,
			"u_material[1].diffuse" ,
			"u_material[1].specular" ,
			"u_material[1].constant" ,
			"u_material[1].linear" ,
			"u_material[1].quadratic" ,
			"u_material[1].shininess"
		},
		{
			"u_material[2].lightPos",
			"u_material[2].ambient" ,
			"u_material[2].diffuse" ,
			"u_material[2].specular" ,
			"u_material[2].constant" ,
			"u_material[2].linear" ,
			"u_material[2].quadratic" ,
			"u_material[2].shininess"
		},
		{
			"u_material[3].lightPos",
			"u_material[3].ambient" ,
			"u_material[3].diffuse" ,
			"u_material[3].specular" ,
			"u_material[3].constant" ,
			"u_material[3].linear" ,
			"u_material[3].quadratic" ,
			"u_material[3].shininess"
		},
		{
			"u_material[4].lightPos",
			"u_material[4].ambient" ,
			"u_material[4].diffuse" ,
			"u_material[4].specular" ,
			"u_material[4].constant" ,
			"u_material[4].linear" ,
			"u_material[4].quadratic" ,
			"u_material[4].shininess"
		},
		{
			"u_material[5].lightPos",
			"u_material[5].ambient" ,
			"u_material[5].diffuse" ,
			"u_material[5].specular" ,
			"u_material[5].constant" ,
			"u_material[5].linear" ,
			"u_material[5].quadratic" ,
			"u_material[5].shininess"
		},
		{
			"u_material[6].lightPos",
			"u_material[6].ambient" ,
			"u_material[6].diffuse" ,
			"u_material[6].specular" ,
			"u_material[6].constant" ,
			"u_material[6].linear" ,
			"u_material[6].quadratic" ,
			"u_material[6].shininess"
		},
		{
			"u_material[7].lightPos",
			"u_material[7].ambient" ,
			"u_material[7].diffuse" ,
			"u_material[7].specular" ,
			"u_material[7].constant" ,
			"u_material[7].linear" ,
			"u_material[7].quadratic" ,
			"u_material[7].shininess"
		},
		{
			"u_material[8].lightPos",
			"u_material[8].ambient" ,
			"u_material[8].diffuse" ,
			"u_material[8].specular" ,
			"u_material[8].constant" ,
			"u_material[8].linear" ,
			"u_material[8].quadratic" ,
			"u_material[8].shininess"
		},
		{
			"u_material[9].lightPos",
			"u_material[9].ambient" ,
			"u_material[9].diffuse" ,
			"u_material[9].specular" ,
			"u_material[9].constant" ,
			"u_material[9].linear" ,
			"u_material[9].quadratic" ,
			"u_material[9].shininess"
		},
		{
			"u_material[10].lightPos",
			"u_material[10].ambient" ,
			"u_material[10].diffuse" ,
			"u_material[10].specular" ,
			"u_material[10].constant" ,
			"u_material[10].linear" ,
			"u_material[10].quadratic" ,
			"u_material[10].shininess"
		},
		{
			"u_material[11].lightPos",
			"u_material[11].ambient" ,
			"u_material[11].diffuse" ,
			"u_material[11].specular" ,
			"u_material[11].constant" ,
			"u_material[11].linear" ,
			"u_material[11].quadratic" ,
			"u_material[11].shininess"
		},
		{
			"u_material[12].lightPos",
			"u_material[12].ambient" ,
			"u_material[12].diffuse" ,
			"u_material[12].specular" ,
			"u_material[12].constant" ,
			"u_material[12].linear" ,
			"u_material[12].quadratic" ,
			"u_material[12].shininess"
		},
		{
			"u_material[13].lightPos",
			"u_material[13].ambient" ,
			"u_material[13].diffuse" ,
			"u_material[13].specular" ,
			"u_material[13].constant" ,
			"u_material[13].linear" ,
			"u_material[13].quadratic" ,
			"u_material[13].shininess"
		},
		{
			"u_material[14].lightPos",
			"u_material[14].ambient" ,
			"u_material[14].diffuse" ,
			"u_material[14].specular" ,
			"u_material[14].constant" ,
			"u_material[14].linear" ,
			"u_material[14].quadratic" ,
			"u_material[14].shininess"
		},
		{
			"u_material[15].lightPos",
			"u_material[15].ambient" ,
			"u_material[15].diffuse" ,
			"u_material[15].specular" ,
			"u_material[15].constant" ,
			"u_material[15].linear" ,
			"u_material[15].quadratic" ,
			"u_material[15].shininess"
		},
		{
			"u_material[16].lightPos",
			"u_material[16].ambient" ,
			"u_material[16].diffuse" ,
			"u_material[16].specular" ,
			"u_material[16].constant" ,
			"u_material[16].linear" ,
			"u_material[16].quadratic" ,
			"u_material[16].shininess"
		},
		{
			"u_material[17].lightPos",
			"u_material[17].ambient" ,
			"u_material[17].diffuse" ,
			"u_material[17].specular" ,
			"u_material[17].constant" ,
			"u_material[17].linear" ,
			"u_material[17].quadratic" ,
			"u_material[17].shininess"
		},
		{
			"u_material[18].lightPos",
			"u_material[18].ambient" ,
			"u_material[18].diffuse" ,
			"u_material[18].specular" ,
			"u_material[18].constant" ,
			"u_material[18].linear" ,
			"u_material[18].quadratic" ,
			"u_material[18].shininess"
		},
		{
			"u_material[19].lightPos",
			"u_material[19].ambient" ,
			"u_material[19].diffuse" ,
			"u_material[19].specular" ,
			"u_material[19].constant" ,
			"u_material[19].linear" ,
			"u_material[19].quadratic" ,
			"u_material[19].shininess"
		},
		{
			"u_material[20].lightPos",
			"u_material[20].ambient" ,
			"u_material[20].diffuse" ,
			"u_material[20].specular" ,
			"u_material[20].constant" ,
			"u_material[20].linear" ,
			"u_material[20].quadratic" ,
			"u_material[20].shininess"
		},
		{
			"u_material[21].lightPos",
			"u_material[21].ambient" ,
			"u_material[21].diffuse" ,
			"u_material[21].specular" ,
			"u_material[21].constant" ,
			"u_material[21].linear" ,
			"u_material[21].quadratic" ,
			"u_material[21].shininess"
		},
		{
			"u_material[22].lightPos",
			"u_material[22].ambient" ,
			"u_material[22].diffuse" ,
			"u_material[22].specular" ,
			"u_material[22].constant" ,
			"u_material[22].linear" ,
			"u_material[22].quadratic" ,
			"u_material[22].shininess"
		},
		{
			"u_material[23].lightPos",
			"u_material[23].ambient" ,
			"u_material[23].diffuse" ,
			"u_material[23].specular" ,
			"u_material[23].constant" ,
			"u_material[23].linear" ,
			"u_material[23].quadratic" ,
			"u_material[23].shininess"
		},
		{
			"u_material[24].lightPos",
			"u_material[24].ambient" ,
			"u_material[24].diffuse" ,
			"u_material[24].specular" ,
			"u_material[24].constant" ,
			"u_material[24].linear" ,
			"u_material[24].quadratic" ,
			"u_material[24].shininess"
		},
		{
			"u_material[25].lightPos",
			"u_material[25].ambient" ,
			"u_material[25].diffuse" ,
			"u_material[25].specular" ,
			"u_material[25].constant" ,
			"u_material[25].linear" ,
			"u_material[25].quadratic" ,
			"u_material[25].shininess"
		},
		{
			"u_material[26].lightPos",
			"u_material[26].ambient" ,
			"u_material[26].diffuse" ,
			"u_material[26].specular" ,
			"u_material[26].constant" ,
			"u_material[26].linear" ,
			"u_material[26].quadratic" ,
			"u_material[26].shininess"
		},
		{
			"u_material[27].lightPos",
			"u_material[27].ambient" ,
			"u_material[27].diffuse" ,
			"u_material[27].specular" ,
			"u_material[27].constant" ,
			"u_material[27].linear" ,
			"u_material[27].quadratic" ,
			"u_material[27].shininess"
		},
		{
			"u_material[28].lightPos",
			"u_material[28].ambient" ,
			"u_material[28].diffuse" ,
			"u_material[28].specular" ,
			"u_material[28].constant" ,
			"u_material[28].linear" ,
			"u_material[28].quadratic" ,
			"u_material[28].shininess"
		},
		{
			"u_material[29].lightPos",
			"u_material[29].ambient" ,
			"u_material[29].diffuse" ,
			"u_material[29].specular" ,
			"u_material[29].constant" ,
			"u_material[29].linear" ,
			"u_material[29].quadratic" ,
			"u_material[29].shininess"
		},
		{
			"u_material[30].lightPos",
			"u_material[30].ambient" ,
			"u_material[30].diffuse" ,
			"u_material[30].specular" ,
			"u_material[30].constant" ,
			"u_material[30].linear" ,
			"u_material[30].quadratic" ,
			"u_material[30].shininess"
		},
		{
			"u_material[31].lightPos",
			"u_material[31].ambient" ,
			"u_material[31].diffuse" ,
			"u_material[31].specular" ,
			"u_material[31].constant" ,
			"u_material[31].linear" ,
			"u_material[31].quadratic" ,
			"u_material[31].shininess"
		} 
	};


}


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
