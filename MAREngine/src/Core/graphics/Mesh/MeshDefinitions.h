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


#ifndef MAR_ENGINE_MESH_DEFINITIONS_H
#define MAR_ENGINE_MESH_DEFINITIONS_H


#include "Vertex.h"


namespace marengine {


<<<<<<< HEAD
	constexpr uint32_t g_MeshStride{ 3 + 3 + 2 + 1 };

	struct TexturePair {

		TexturePair(uint32_t i, std::string str) :
			bindingIndex(i),
			texturePath(str)
		{}

		uint32_t bindingIndex{ 0 };
		std::string texturePath{};

	};

=======
	struct FTexturePair {
		FTexturePair(uint32_t bd, std::string tp) :
			bindingIndex(bd),
			texturePath(std::move(tp))
		{}

		uint32_t bindingIndex{ 0 };
		std::string texturePath{ "" };
	};


>>>>>>> renderissue
	typedef std::vector<Vertex> FVertexArray;
	typedef std::vector<uint32_t> FIndicesArray;
	typedef std::vector<maths::mat4> FTransformsArray;
	typedef std::vector<maths::vec4> FColorsArray;
<<<<<<< HEAD
	typedef std::vector<TexturePair> FTextureArray;
=======
	typedef std::vector<FTexturePair> FTexturesArray;

	constexpr uint32_t g_MeshStride{ 3 + 3 + 2 + 1 };
>>>>>>> renderissue


}



<<<<<<< HEAD

#endif // !MAR_ENGINE_RENDER_API_DEFINITIONS_H
=======
#endif // !MAR_ENGINE_MESH_DEFINITIONS_H
>>>>>>> renderissue
