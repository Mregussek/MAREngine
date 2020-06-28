/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "../../../mar.h"

namespace mar {
	namespace graphics {


		struct {
			const char* mrTex = "resources/textures/mr.jpg";
			const char* yellowTex = "resources/textures/yellow-texture.jpg";
			const char* grassTex = "resources/textures/grass-texture.jpg";
			const char* wallTex = "resources/textures/wall.jpg";
			const char* blueTex = "resources/textures/blue-texture.jpg";
			const char* redTex = "resources/textures/red-texture.jpg";
			const char* blackTex = "resources/textures/black-texture.jpg";

			const std::vector<const char*> SkyboxFaces{
				"resources/textures/skybox/right.jpg" ,
				"resources/textures/skybox/left.jpg" ,
				"resources/textures/skybox/top.jpg" ,
				"resources/textures/skybox/bottom.jpg" ,
				"resources/textures/skybox/front.jpg" ,
				"resources/textures/skybox/back.jpg" ,
			};

			const std::vector<const char*> mc_grass_faces{
				 "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass-up.jpg"
				 ,  "resources/textures/mc/dirt.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
			};

			const std::vector<const char*> mc_cobble_faces{
				"resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
			};

			const std::vector<const char*> mc_diamond_faces{
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg"
			};
		} TexturePaths;


} }