/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MESH_H
#define MESH_H

#include "../../mar.h"
#include "Shapes/Shapes.h"

namespace mar {

    class Mesh {
    public:
        Mesh() = default;

        static void extendID(Shapes* shape, const float& nextID);

        static void rotateObject(Shapes* shape, const glm::vec3& angle);
        static std::vector<float> rotateObject(const unsigned int& size, const unsigned int& stride,
            const glm::vec3& angle, const glm::vec3& center, const std::vector<float>& passedValue);

        static void changeCenterOfObject(Shapes* shape, const glm::vec3& center);
        static std::vector<float> changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
            const glm::vec3& center, const std::vector<float>& passedValue);

        static void changeIndicesFormat(Shapes* shape, unsigned int& max_value);
        static std::vector<unsigned int> changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                    const std::vector<unsigned int>& passedValue);

        static glm::mat4 getRotationMatrix(const glm::vec3& center, const glm::vec3& angle);
        static glm::mat4 getRotationMatrix(const glm::vec3& center, const float& angle);
    };
}

#endif // !MESH_H