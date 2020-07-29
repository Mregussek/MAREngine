/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_TEXTURE_PATHS_H
#define MAR_ENGINE_TEXTURE_PATHS_H

#include "../../../mar.h"

namespace mar {
	

		struct TexturePaths {
			inline static const char* empty = "resources/textures/empty";

			inline static const char* mrTex = "resources/textures/mr.jpg";
			inline static const char* yellowTex = "resources/textures/yellow-texture.jpg";
			inline static const char* grassTex = "resources/textures/grass-texture.jpg";
			inline static const char* wallTex = "resources/textures/wall.jpg";
			inline static const char* blueTex = "resources/textures/blue-texture.jpg";
			inline static const char* redTex = "resources/textures/red-texture.jpg";
			inline static const char* blackTex = "resources/textures/black-texture.jpg";

			inline static const char* mc_diamond = "resources/textures/diamond";
			inline static const char* mc_cobble = "resources/textures/cobble";
			inline static const char* mc_dirt = "resources/textures/dirt";
			inline static const char* mc_grass = "resources/textures/grass";
		};


}

#endif // !MAR_ENGINE_TEXTURE_PATHS_H