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


#ifndef MAR_ENGINE_GRAPHICS_SHAPEMANIPULATOR_H
#define MAR_ENGINE_GRAPHICS_SHAPEMANIPULATOR_H


#include "../../../mar.h"


namespace mar {
    namespace graphics {


        class ShapeManipulator {
        public:

            static void extendShapeID(std::vector<float>& vertices, uint32_t stride, float newid);
  
            static void extendIndices(std::vector<uint32_t>& indices, uint32_t extension);
            static void extendIndices(std::vector<uint32_t>& indices, uint32_t start, uint32_t end, uint32_t extension);

            static void calculateNormals(std::vector<float>& vertices, const std::vector<uint32_t>& indices, const int32_t stride);
        
        };


} }

#endif // !MAR_ENGINE_SHAPEMANIPULATOR_H