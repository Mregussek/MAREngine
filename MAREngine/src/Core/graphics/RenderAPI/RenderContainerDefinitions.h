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


#ifndef MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_DEFINITIONS_H
#define MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_DEFINITIONS_H


#include "../../../mar.h"
#include "../Mesh/Vertex.h"


namespace marengine {

	typedef std::pair<int32_t, maths::vec4> ColorPair;
	typedef std::pair<int32_t, std::string> TexturePair;

	typedef std::vector<Vertex> VertexVector;
	typedef std::vector<uint32_t> IndicesVector;
	typedef std::vector<maths::mat4> Mat4Vector;
	typedef std::vector<ColorPair> ColorVector;
	typedef std::vector<TexturePair> TextureVector;
	typedef std::vector<float> FloatVector;

}

#endif // !MAR_ENGINE_GRAPHICS_RENDER_CONTAINER_DEFINITIONS_H
