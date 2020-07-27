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

            static std::vector<float> getVertices_Cube() {
                return {
                    //  front (x, y, z)		// LightNormal			// Texture		// ShapeIndex
                    -1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 0
                     1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 1
                     1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, // 2
                    -1.0f,  1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 3
                    //  back 							
                    -1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 4
                     1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 5
                     1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, // 6
                    -1.0f,  1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f  // 7
                };
            }
            
            static std::vector<uint32_t> getIndices_Cube() {
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

            static std::vector<float> getVertices_Wall() {
                return {
                    //  front (x, y, z)			// LightNormal			// Texture		// ShapeIndex
                    -0.2f, -1.0f,  10.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 0
                     0.2f, -1.0f,  10.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		0.0f, // 1
                     0.2f,  5.0f,  10.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 2
                    -0.2f,  5.0f,  10.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,		0.0f, // 3
                    //  back 								
                    -0.2f, -1.0f, -10.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 4
                     0.2f, -1.0f, -10.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, // 5
                     0.2f,  5.0f, -10.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, // 6
                    -0.2f,  5.0f, -10.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f  // 7
                };
            }

            static std::vector<uint32_t> getIndices_Wall() {
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

            static std::vector<float> getVertices_Surface() {
                return {
                    // (x, y, z)			// LightNormal			// TextureCoords	// ShapeIndex
                    -15.0f, -1.0f,  15.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,			0.0f, // 0
                     15.0f, -1.0f,  15.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,			0.0f, // 1
                     15.0f, -1.0f, -15.0f,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,			0.0f, // 2
                    -15.0f, -1.0f, -15.0f,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,			0.0f  // 3
                };
            }

            static std::vector<uint32_t> getIndices_Surface() {
                return {
                        0, 1, 2, // first triangle
                        2, 3, 0  // second triangle
                };
            }

            static std::vector<float> getVertices_Pyramid() {
                return {
                    // (x, y, z)			// LightNormal			// TextureCoords // ShapeIndex
                    -1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		 0.0f,
                     1.0f, -1.0f,  1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		 0.0f,
                     1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,		 0.0f,
                    -1.0f, -1.0f, -1.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,		 0.0f,
                     0.0f,  1.0f,  0.0f,	0.0f, 0.0f, 1.0f,		0.5f, 0.5f,		 0.0f
                };
            }

            static std::vector<uint32_t> getIndices_Pyramid() {
                return {
                    0, 1, 2,	2, 3, 0, // fundamental quad
                    0, 1, 4,	1, 2, 4, // side triangles
                    2, 3, 4,	3, 0, 4
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