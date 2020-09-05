/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GRAPHICS_MESH_CREATOR_H
#define MAR_ENGINE_GRAPHICS_MESH_CREATOR_H


#include "../../../mar.h"
#include "../GraphicsLogs.h"
#include "ShapeManipulator.h"
#include "../../ecs/ECS/EntityCollection.h"
#include "../../ecs/ECS/Entity.h"
#include "../../ecs/ECS/Components.h"


namespace mar {
    namespace graphics {


        class MeshCreator {
        public:

            struct Cube {
                static std::string getID() { return "Cube"; }
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            struct Pyramid {
                static std::string getID() { return "Pyramid"; }
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            struct Wall {
                static std::string getID() { return "Wall"; }
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            }; 

            struct Surface {
                static std::string getID() { return "Surface"; }
                static std::vector<float> getVertices();
                static std::vector<uint32_t> getIndices();
            };

            static void loadOBJ(std::string filename, std::string path, ecs::EntityCollection& collection);
        };


} }


#endif // !MAR_ENGINE_MESH_CREATOR_H