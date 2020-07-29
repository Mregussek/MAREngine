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

        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H