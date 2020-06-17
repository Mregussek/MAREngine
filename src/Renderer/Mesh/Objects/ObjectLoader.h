/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_OBJECT_LOADER_H
#define MAR_ENGINE_OBJECT_LOADER_H

#include "../../../mar.h"

namespace mar {
    namespace graphics { namespace objects {

        class ObjectLoader {
            static std::string s_name;
            static std::vector<float> s_vertices;
            static std::vector<unsigned int> s_indices;
            static std::vector<unsigned int> s_layout;
            static glm::vec3 s_center;
            static glm::vec3 s_angle;
            static glm::vec3 s_defaultcolor;
            static unsigned int s_id;
            static unsigned int s_texid;

        public:
            static void loadObject(const std::string& path);

            // --- GET METHODS --- //
            inline static std::string getName() { return s_name; }
            inline static std::vector<float> getVertices() { return s_vertices; }
            inline static std::vector<unsigned int> getIndices() { return s_indices; }
            inline static std::vector<unsigned int> getLayout() { return s_layout; }
            inline static unsigned int getID() { return s_id; }
            inline static unsigned int getTextureID() { return s_texid; }
            inline static glm::vec3 getCenter() { return s_center; }
            inline static glm::vec3 getAngle() { return s_angle; }
        };


} } }

#endif // !MAR_ENGINE_OBJECT_LOADER_H
