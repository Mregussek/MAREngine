/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {

    void Mesh::extendID(Cube* cube, const float& nextID) {
        unsigned int size = cube->getSizeofVertices();
        unsigned int stride = cube->getStride();

        // extend all vertices, which defines texture id
        for (unsigned int j = 1; j < size / stride; j++)
            cube->changeVerticesIndex(j * stride - 1, nextID);

        cube->setID(nextID);
    }

    void Mesh::rotateObject(Cube* cube, const glm::vec3& angle) {
        cube->verticesVector = rotateObject(cube->getSizeofVertices(), cube->getStride(), angle, cube->getCenter(), cube->verticesVector);
    }

    std::vector<float> Mesh::rotateObject(const unsigned int& size, const unsigned int& stride,
        const glm::vec3& angle, const glm::vec3& center, std::vector<float>& passedValue) {

        // prescribe values to vec4 (for multiplying with matrix4x4)
        glm::vec4 positions[8];
        for (unsigned int j = 0; j < size / stride; j++) {
            positions[j].x = passedValue.at(j * stride + 0);
            positions[j].y = passedValue.at(j * stride + 1);
            positions[j].z = passedValue.at(j * stride + 2);
            positions[j].w = 1.0f;
        }

        // create transform matrix
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), center);
        glm::mat4 invTranslate = glm::inverse(translate);
        glm::mat4 rotationMatrix = 
              glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), { 1.0f, 0.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), { 0.0f, 1.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), { 0.0f, 0.0f, 1.0f });

        glm::mat4 transform = translate * rotationMatrix * invTranslate;

        std::vector<float> returnValue(passedValue.size());

        for (unsigned int j = 0; j < size / stride; j++) {
            glm::vec4 result = positions[j] * transform;

            returnValue[j * stride + 0] = result.x;
            returnValue[j * stride + 1] = result.y;
            returnValue[j * stride + 2] = result.z;

            for (unsigned int k = 3; k < stride; k++)
                returnValue[j * stride + k] = passedValue[j * stride + k];
        }

        return returnValue;
    }

    void Mesh::changeCenterOfObject(Cube* cube, const glm::vec3& center) {
        cube->verticesVector = changeCenterOfObject(cube->getSizeofVertices(), cube->getStride(), center, cube->verticesVector);
        cube->prescribeCenter(center);
    }

    std::vector<float> Mesh::changeCenterOfObject(const unsigned int& size, const unsigned int& stride, 
                const glm::vec3& center, std::vector<float>& passedValue) {
        int i = 0;
        bool back = false;
        std::vector<float> returnValue(passedValue.size());

        for (unsigned int j = 0; j < size / stride; j++) {
            if (!back) {
                if (i == 0) {
                    returnValue[j * stride] = center.x - 1;
                    returnValue[j * stride + 1] = center.y - 1;
                    returnValue[j * stride + 2] = center.z + 1;
                    i++;
                } else if (i == 1) {
                    returnValue[j * stride] = center.x + 1;
                    returnValue[j * stride + 1] = center.y - 1;
                    returnValue[j * stride + 2] = center.z + 1;
                    i++;
                } else if (i == 2) {
                    returnValue[j * stride] = center.x + 1;
                    returnValue[j * stride + 1] = center.y + 1;
                    returnValue[j * stride + 2] = center.z + 1;
                    i++;
                } else {
                    returnValue[j * stride] = center.x - 1;
                    returnValue[j * stride + 1] = center.y + 1;
                    returnValue[j * stride + 2] = center.z + 1;
                    i = 0;
                    back = true;
                }
            } else {
                if (i == 0) {
                    returnValue[j * stride] = center.x - 1;
                    returnValue[j * stride + 1] = center.y - 1;
                    returnValue[j * stride + 2] = center.z - 1;
                    i++;
                } else if (i == 1) {
                    returnValue[j * stride] = center.x + 1;
                    returnValue[j * stride + 1] = center.y - 1;
                    returnValue[j * stride + 2] = center.z - 1;
                    i++;
                } else if (i == 2) {
                    returnValue[j * stride] = center.x + 1;
                    returnValue[j * stride + 1] = center.y + 1;
                    returnValue[j * stride + 2] = center.z - 1;
                    i++;
                } else {
                    returnValue[j * stride] = center.x - 1;
                    returnValue[j * stride + 1] = center.y + 1;
                    returnValue[j * stride + 2] = center.z - 1;
                    i = 0;
                }
            }

            for (unsigned int k = 3; k < stride; k++) 
                returnValue[j * stride + k] = passedValue[j * stride + k];   
        }

        return returnValue;
    }

    void Mesh::changeIndicesFormat(Cube* cube, unsigned int& max_value) {
        changeIndicesFormat(cube->getSizeofIndices(), max_value, cube->indicesVector);
    }

    void Mesh::changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                     std::vector<unsigned int>& returnValue) {
        
            for (unsigned int i = 0; i < size; i++)
                returnValue[i] += max_value;
    }
}