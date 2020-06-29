/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_TEXTURE_PATHS_H
#define MAR_ENGINE_TEXTURE_PATHS_H

#include "../../../mar.h"

namespace mar {
	

		struct TexturePaths {
			inline static const char* mrTex = "resources/textures/mr.jpg";
			inline static const char* yellowTex = "resources/textures/yellow-texture.jpg";
			inline static const char* grassTex = "resources/textures/grass-texture.jpg";
			inline static const char* wallTex = "resources/textures/wall.jpg";
			inline static const char* blueTex = "resources/textures/blue-texture.jpg";
			inline static const char* redTex = "resources/textures/red-texture.jpg";
			inline static const char* blackTex = "resources/textures/black-texture.jpg";

			inline static const std::vector<const char*> SkyboxFaces{
				"resources/textures/skybox/right.jpg" ,
				"resources/textures/skybox/left.jpg" ,
				"resources/textures/skybox/top.jpg" ,
				"resources/textures/skybox/bottom.jpg" ,
				"resources/textures/skybox/front.jpg" ,
				"resources/textures/skybox/back.jpg" ,
			};

			inline static const std::vector<const char*> mc_grass_faces{
				 "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass-up.jpg"
				 ,  "resources/textures/mc/dirt.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
			};

			inline static const std::vector<const char*> mc_cobble_faces{
				"resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
			};

			inline static const std::vector<const char*> mc_diamond_faces{
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg",
				"resources/textures/mc/mc-diamond.jpg"
			};
		};


}

#endif // !MAR_ENGINE_TEXTURE_PATHS_H