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


#ifndef MAR_ENGINE_GRAPHICS_GRAPHICS_LIMITS_H
#define MAR_ENGINE_GRAPHICS_GRAPHICS_LIMITS_H


#include "../../mar.h"
#include "Mesh/Vertex.h"


namespace mar::graphics {


	struct GraphicLimits {
			
		const static inline uint32_t maxTrianglesCount{ 100000 };
		const static inline uint32_t maxVerticesCount{ maxTrianglesCount * 3 };
		const static inline uint32_t maxIndicesCount{ maxTrianglesCount * 3 };
		const static inline uint32_t sizeOfVertices{ maxVerticesCount * sizeof(Vertex) };
		const static inline uint32_t sizeOfIndices{ maxIndicesCount * sizeof(uint32_t) };

	};


}


#endif // !MAR_ENGINE_GRAPHICS_GRAPHICS_LIMITS_H