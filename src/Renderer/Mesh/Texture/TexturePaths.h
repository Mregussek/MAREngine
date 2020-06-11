/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "../../../mar.h"

namespace mar {
	namespace graphics {


		struct {
#ifndef IMPORTED_FROM_GITHUB 
			const std::string mrTex = "resources/textures/mr.jpg";
			const std::string yellowTex = "resources/textures/yellow-texture.jpg";
			const std::string grassTex = "resources/textures/grass-texture.jpg";
			const std::string wallTex = "resources/textures/wall.jpg";
			const std::string blueTex = "resources/textures/blue-texture.jpg";
			const std::string redTex = "resources/textures/red-texture.jpg";
			const std::string blackTex = "resources/textures/black-texture.jpg";

			const std::vector<std::string> SkyboxFaces{
				"resources/textures/skybox/right.jpg" ,
				"resources/textures/skybox/left.jpg" ,
				"resources/textures/skybox/top.jpg" ,
				"resources/textures/skybox/bottom.jpg" ,
				"resources/textures/skybox/front.jpg" ,
				"resources/textures/skybox/back.jpg" ,
			};

			const std::vector<std::string> mc_grass_faces{
				 "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass-up.jpg"
				 ,  "resources/textures/mc/dirt.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
			};

			const std::vector<std::string> mc_cobble_faces{
				"resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
			};

			const std::vector<std::string> mc_diamond_faces{
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg"
			};

#else
			const std::string mrTex = "../../../resources/textures/mr.jpg";
			const std::string yellowTex = "../../../resources/textures/yellow-texture.jpg";
			const std::string grassTex = "../../../resources/textures/grass-texture.jpg";
			const std::string wallTex = "../../../resources/textures/wall.jpg";
			const std::string blueTex = "../../../resources/textures/blue-texture.jpg";
			const std::string redTex = "../../../resources/textures/red-texture.jpg";
			const std::string blackTex = "../../../resources/textures/black-texture.jpg";

			const std::vector<std::string> SkyboxFaces{
				"../../../resources/textures/skybox/right.jpg" ,
				"../../../resources/textures/skybox/left.jpg" ,
				"../../../resources/textures/skybox/top.jpg" ,
				"../../../resources/textures/skybox/bottom.jpg" ,
				"../../../resources/textures/skybox/front.jpg" ,
				"../../../resources/textures/skybox/back.jpg" ,
			};

			const std::vector<std::string> mc_grass_faces{
				 "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass-up.jpg"
				 ,  "../../../resources/textures/mc/dirt.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
			};

			const std::vector<std::string> mc_cobble_faces{
				"../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
			};

			const std::vector<std::string> mc_diamond_faces{
				"../../../resources/textures/mc/mc-diamond.jpg",
				"../../../resources/textures/mc/mc-diamond.jpg",
				"../../../resources/textures/mc/mc-diamond.jpg",
				"../../../resources/textures/mc/mc-diamond.jpg",
				"../../../resources/textures/mc/mc-diamond.jpg",
				"../../../resources/textures/mc/mc-diamond.jpg"
			};
#endif
		} TexturePaths;


} }