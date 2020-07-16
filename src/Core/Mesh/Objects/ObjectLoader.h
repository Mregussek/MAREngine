/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_OBJECT_LOADER_H
#define MAR_ENGINE_OBJECT_LOADER_H

#include "../../../mar.h"
#include "../../../Debug/Log.h"

namespace mar {
    namespace graphics { namespace objects {


        class ObjectLoader {
            static const char* s_name;
            static std::vector<float> s_vertices;
            static std::vector<uint32_t> s_indices;
            static std::vector<uint32_t> s_layout;
            static maths::vec3 s_center;
            static maths::vec3 s_angle;
            static maths::vec3 s_scale;
            static maths::vec3 s_defaultcolor;
            static float s_id;
            static float s_texid;

        public:
            static void loadObject(const char* path);

            // --- GET METHODS --- //
            inline static const char* getName() { return s_name; }
            inline static std::vector<float> getVertices() { return s_vertices; }
            inline static std::vector<uint32_t> getIndices() { return s_indices; }
            inline static std::vector<uint32_t> getLayout() { return s_layout; }
            inline static float getID() { return s_id; }
            inline static float getTextureID() { return s_texid; }
            inline static maths::vec3 getCenter() { return s_center; }
            inline static maths::vec3 getAngle() { return s_angle; }
            inline static maths::vec3 getScale() { return s_scale; }
            inline static maths::vec3 getColor() { return s_defaultcolor; }
        };


} } }

#endif // !MAR_ENGINE_OBJECT_LOADER_H
