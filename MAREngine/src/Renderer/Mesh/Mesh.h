/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MESH_H
#define MESH_H

#include "../../mar.h"
#include "Shapes/Shapes.h"
#include "Shapes/Cube.h"

namespace mar {

    class Mesh {
    public:
        Mesh() = default;

        static void extendID(std::shared_ptr<Shapes>& shape, const float& nextID);

        static void rotateObject(std::shared_ptr<Shapes>& shape, const glm::vec3& angle);
        static std::vector<float> rotateObject(const unsigned int& size, const unsigned int& stride,
            const glm::vec3& angle, const glm::vec3& center, std::vector<float>& returnValue);

        static void changeCenterOfObject(std::shared_ptr<Shapes>& shape, const glm::vec3& center);
        static std::vector<float> changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
                    const glm::vec3& center, std::vector<float>& returnValue);

        static void changeIndicesFormat(std::shared_ptr<Shapes>& shape, unsigned int& max_value);
        static void changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                    std::vector<unsigned int>& returnValue);
    };
}

#endif // !MESH_H