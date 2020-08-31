/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
#define MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H


#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		namespace ShaderUniforms {

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

			inline static const std::vector<Material> u_material{
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
				}
			};

			inline static const std::vector<std::string> u_SeparateColor = {
				"u_SeparateColor[0]",
				"u_SeparateColor[1]",
				"u_SeparateColor[2]",
				"u_SeparateColor[3]",
				"u_SeparateColor[4]",
				"u_SeparateColor[5]",
				"u_SeparateColor[6]",
				"u_SeparateColor[7]",
				"u_SeparateColor[8]",
				"u_SeparateColor[9]",
				"u_SeparateColor[10]",
				"u_SeparateColor[11]",
				"u_SeparateColor[12]",
				"u_SeparateColor[13]",
				"u_SeparateColor[14]",
				"u_SeparateColor[15]",
				"u_SeparateColor[16]",
				"u_SeparateColor[17]",
				"u_SeparateColor[18]",
				"u_SeparateColor[19]",
				"u_SeparateColor[20]",
				"u_SeparateColor[21]",
				"u_SeparateColor[22]",
				"u_SeparateColor[23]",
				"u_SeparateColor[24]",
				"u_SeparateColor[25]",
				"u_SeparateColor[26]",
				"u_SeparateColor[27]",
				"u_SeparateColor[28]",
				"u_SeparateColor[29]",
				"u_SeparateColor[30]",
				"u_SeparateColor[31]",
				"u_SeparateColor[32]"
			};

		};


} }


#endif // !MAR_ENGINE_PLATFORMS_SHADER_UNIFORMS_H
