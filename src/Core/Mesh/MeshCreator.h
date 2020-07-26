/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_MESH_CREATOR_H
#define MAR_ENGINE_MESH_CREATOR_H


#include "../../mar.h"
#include "Shapes/ShapeManipulator.h"
#include "Texture/TexturePaths.h"


namespace mar {
    namespace graphics {


        class MeshCreator {
        public:
            static Ref<Shape> createEmptyShape() {
                return PointerFactory<Shape>::makeRef();
            }

            static Ref<Shape> createCube() {
                return PointerFactory<Cube>::makeRef();
            }

            static Ref<Shape> createWall() {
                return PointerFactory<Wall>::makeRef();
            }

            static Ref<Shape> createSurface() {
                return PointerFactory<Surface>::makeRef();
            }

            static Ref<Shape> createPyramid() {
                return PointerFactory<Pyramid>::makeRef();
            }

            static std::vector<float> getVertices() {
                return {
                    //  front (x, y, z)		// LightNormal			// Texture		// Texture Index	// ShapeIndex
                    -1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f,				0.0f, // 0
                     1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f,				0.0f, // 1
                     1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,				0.0f, // 2
                    -1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f,				0.0f, // 3
                    //  back 							
                    -1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f,				0.0f, // 4
                     1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f,				0.0f, // 5
                     1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f,				0.0f, // 6
                    -1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f,				0.0f  // 7
                };
            }
            
            static std::vector<float> getIndices() {
                return {
                    // front	// back
                    0, 1, 2,	7, 6, 5,
                    2, 3, 0,	5, 4, 7,
                    // right	// left
                    1, 5, 6,	4, 0, 3,
                    6, 2, 1,	3, 7, 4,
                    // bottom	// top
                    4, 5, 1,	3, 2, 6,
                    1, 0, 4,	6, 7, 3
                };
            }

            static void moveShape(Ref<Shape>& ref_shape, Ref<Shape>& shape_to_move) {
                ref_shape->setName(shape_to_move->getName());
                ref_shape->setID(shape_to_move->getID());
                ref_shape->setTextureID(shape_to_move->getTextureID());
                ref_shape->setUsedTexture(shape_to_move->getUsedTexture());
                ref_shape->setUsedObj(shape_to_move->getUsedObj());
                ref_shape->setCenter(shape_to_move->getCenter());
                ref_shape->setAngle(shape_to_move->getAngle());
                ref_shape->setScaleVec(shape_to_move->getScaleVec());
                ref_shape->setVertices(shape_to_move->getVertices());
                ref_shape->setIndices(shape_to_move->getIndices());
            }

        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H