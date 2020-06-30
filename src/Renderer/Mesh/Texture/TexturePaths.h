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

			inline static const char* mc_diamond = "resources/textures/mc/diamond";
			inline static const char* mc_cobble = "resources/textures/mc/cobble";
			inline static const char* mc_dirt = "resources/textures/mc/dirt";
			inline static const char* mc_grass = "resources/textures/mc/grass";
		};


}

#endif // !MAR_ENGINE_TEXTURE_PATHS_H