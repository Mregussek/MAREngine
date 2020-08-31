/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_MESH_CREATOR_H
#define MAR_ENGINE_GRAPHICS_MESH_CREATOR_H


#include "../../../mar.h"
#include "../GraphicsLogs.h"
#include "ShapeManipulator.h"


namespace mar {
    namespace graphics {


        class MeshCreator {
        public:

            struct Cube {
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            struct Pyramid {
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            struct Wall {
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            }; 

            struct Surface {
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            struct OBJ {
                static std::vector<float> vertices;
                static std::vector<uint32_t> indices;

                static void loadOBJ(const char* filename);

                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H