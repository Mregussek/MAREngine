/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_OBJECT_H
#define MAR_ENGINE_OBJECT_H

#include "../mar.h"
#include "ObjectLoader.h"

namespace mar {
    namespace graphics { namespace objects {

        class Object {
            std::string m_name;
            std::vector<float> m_vertices;
            std::vector<unsigned int> m_indices;
            std::vector<unsigned int> m_layout;
            glm::vec3 m_center;
            glm::vec3 m_angle;
            unsigned int m_id;
            unsigned int m_texid;

        public:
            Object() = default;

            void assignDataFromFile(const std::string& path);

            // --- GET METHODS --- //
            inline std::string& getName() { return m_name; }
            inline std::vector<float>& getVertices() { return m_vertices; }
            inline std::vector<unsigned int>& getIndices() { return m_indices; }
            inline std::vector<unsigned int>& getLayout() { return m_layout; }
            inline glm::vec3& getCenter() { return m_center; }
            inline glm::vec3& getAngle() { return m_angle; }
            inline unsigned int& getID() { return m_id; }
            inline unsigned int& getTextureID() { return m_texid; }
        };


} } }

#endif // !MAR_ENGINE_OBJECT_H
