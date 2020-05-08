/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {

    void Mesh::extendID(Shapes* shape, const float& nextID) {
        if (nextID == 0.0f) return;

        unsigned int size = shape->getSizeofVertices();
        unsigned int stride = shape->getStride();

        // extend all vertices, which defines texture id
        for (unsigned int j = 1; j < size / stride + 1; j++)
            shape->setVertice(j * stride - 1, nextID);

        shape->setID(nextID);
    }

    void Mesh::rotateObject(Shapes* shape, const glm::vec3& angle) {
        if (angle == shape->getAngle()) return;

        std::vector<float> new_vertices = rotateObject(shape->getSizeofVertices(), shape->getStride(), angle, shape->getCenter(), shape->getVerticesVector());
        shape->setVerticesVector(new_vertices);
        shape->setAngle(angle);
    }

    std::vector<float> Mesh::rotateObject(const unsigned int& size, const unsigned int& stride,
        const glm::vec3& angle, const glm::vec3& center, const std::vector<float>& passedValue) {

        // prescribe values to vec4 (for multiplying with matrix4x4)
        glm::vec4 positions[8];
        for (unsigned int j = 0; j < size / stride; j++) {
            positions[j].x = passedValue.at(j * stride + 0);
            positions[j].y = passedValue.at(j * stride + 1);
            positions[j].z = passedValue.at(j * stride + 2);
            positions[j].w = 1.0f;
        }

        // create transform matrix
        glm::mat4 rotationMatrix = 
              glm::rotate(glm::mat4(1.0f), glm::radians(angle.x), { 1.0f, 0.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(angle.y), { 0.0f, 1.0f, 0.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(angle.z), { 0.0f, 0.0f, 1.0f });

        std::vector<float> returnValue(passedValue.size());

        for (unsigned int j = 0; j < size / stride; j++) {
            glm::vec4 result = positions[j] * rotationMatrix;

            returnValue[j * stride + 0] = result.x;
            returnValue[j * stride + 1] = result.y;
            returnValue[j * stride + 2] = result.z;

            for (unsigned int k = 3; k < stride; k++)
                returnValue[j * stride + k] = passedValue[j * stride + k];
        }

        return returnValue;
    }

    void Mesh::changeCenterOfObject(Shapes* shape, const glm::vec3& center) {
        if (center == shape->getCenter()) return;

        std::vector<float> new_vertices = changeCenterOfObject(shape->getSizeofVertices(), shape->getStride(), center, shape->getVerticesVector());
        shape->setVerticesVector(new_vertices);
        shape->setCenter(center);
    }

   std::vector<float> Mesh::changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
        const glm::vec3& center, const std::vector<float>& passedValue) {

        glm::vec4 positions[8];
        for (unsigned int j = 0; j < size / stride; j++) {
            positions[j].x = passedValue.at(j * stride + 0);
            positions[j].y = passedValue.at(j * stride + 1);
            positions[j].z = passedValue.at(j * stride + 2);
            positions[j].w = 1.0f;
        }

        glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), center);
        std::vector<float> returnValue(passedValue.size());

        for (unsigned int j = 0; j < size / stride; j++) {
            glm::vec4 result = positions[j] * translateMatrix;

            returnValue[j * stride + 0] = result.x;
            returnValue[j * stride + 1] = result.y;
            returnValue[j * stride + 2] = result.z;

            for (unsigned int k = 3; k < stride; k++)
                returnValue[j * stride + k] = passedValue[j * stride + k];
        }

        return returnValue;
    }

    void Mesh::changeIndicesFormat(Shapes* shape, unsigned int& max_value) {
        if (max_value == 0) return;

        std::vector<unsigned int> new_indices = changeIndicesFormat(shape->getSizeofIndices(), max_value, shape->getIndicesVector());
        shape->setIndicesVector(new_indices);
    }

    std::vector<unsigned int> Mesh::changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                     const std::vector<unsigned int>& passedValue) {
        
        std::vector<unsigned int> returnValue(passedValue.size());

        for (unsigned int i = 0; i < size; i++)
            returnValue[i] = passedValue[i] + max_value;

        return returnValue;
    }

    glm::mat4 Mesh::getRotationMatrix(const glm::vec3& center, const glm::vec3& angle) {
        glm::mat4 rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 transform = glm::rotate(rotation, glm::radians(angle.x), glm::vec3(1.0f, 0.0f, 0.0f))
            * glm::rotate(rotation, glm::radians(angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::rotate(rotation, glm::radians(angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 inverse = glm::translate(glm::mat4(1.0f), center);
        return rotation * transform * inverse;
    }
}