/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_CREATOR_H
#define MAR_ENGINE_MESH_CREATOR_H


#include "../../mar.h"
#include "Shapes/Shapes.h"
#include "Shapes/DefaultShapes/Cube.h"
#include "Shapes/DefaultShapes/Wall.h"
#include "Shapes/DefaultShapes/Surface.h"
#include "Shapes/DefaultShapes/Pyramid.h"
#include "Mesh.h"


namespace mar {
    namespace graphics {


        class MeshCreator {
        public:
            static std::shared_ptr<Shape> createCube() {
                return std::make_shared<Cube>();
            }

            static std::shared_ptr<Shape> createWall() {
                return std::make_shared<Wall>();
            }

            static std::shared_ptr<Shape> createSurface() {
                return std::make_shared<Surface>();
            }

            static std::shared_ptr<Shape> createPyramid() {
                return std::make_shared<Pyramid>();
            }

        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H