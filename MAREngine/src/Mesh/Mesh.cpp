/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    Mesh::Mesh(Cube& shape)
        : vao(VertexArray()),
        vbo(VertexBuffer(shape.verticesVector)),
        ebo(ElementBuffer(shape.indicesVector)),
        lay(VertexBufferLayout())
    {
        for (auto const& l : shape.layout) lay.push<float>(l);

        vao.addBuffer(vbo, lay);
    }

    Mesh::Mesh(const std::vector<Cube>& shapes, const std::vector<glm::vec3>& centers)
        : vao(VertexArray()),
        lay(VertexBufferLayout())
    {
        for (unsigned int i = 0; i < shapes.size(); i++) {
            changeCenterOfObject(shapes[i].vertices, shapes[i].getSizeofVertices(), shapes[i].getStride(), centers[i], _vertices);
            changeIndicesOfObject(shapes[i].indices, shapes[i].getSizeofIndices(), shapes[i].getStride(), _indices, i);
        }

        // Run initialization
        vbo = VertexBuffer(_vertices);
        ebo = ElementBuffer(_indices);

        for (auto const& l : shapes[0].layout) lay.push<float>(l);

        vao.addBuffer(vbo, lay);
    }

    void Mesh::bind() {
        vao.bind();
        ebo.bind();
    }

    void Mesh::unbind() {
        vao.unbind();
        vbo.unbind();
        ebo.unbind();
    }

    void Mesh::changeIndicesOfObject(const unsigned int* indices, const unsigned int& size, const unsigned int& stride, std::vector<unsigned int>& returnValue, const int& indexOfObject) {
        if (indexOfObject == 0) {
            for (unsigned int i = 0; i < size; i++)
                returnValue.push_back(indices[i]);

            return;
        }

        unsigned int add = size / stride;
        for (unsigned int i = 0; i < size; i++) {
            returnValue.push_back(indices[i] + add - 1);
        }
    }

    void Mesh::changeCenterOfObject(const float* vertices, const unsigned int& size, const unsigned int& stride, const glm::vec3& center, std::vector<float>& returnValue) {
        int i = 0;
        bool back = false;
        returnValue.clear();

        for (unsigned int j = 0; j < size / stride; j++) {
            if (!back) {
                if (i == 0) {
                    returnValue.push_back(center.x - 1);
                    returnValue.push_back(center.y - 1);
                    returnValue.push_back(center.z + 1);
                    i++;
                } else if (i == 1) {
                    returnValue.push_back(center.x + 1);
                    returnValue.push_back(center.y - 1);
                    returnValue.push_back(center.z + 1);
                    i++;
                } else if (i == 2) {
                    returnValue.push_back(center.x + 1);
                    returnValue.push_back(center.y + 1);
                    returnValue.push_back(center.z + 1);
                    i++;
                } else {
                    returnValue.push_back(center.x - 1);
                    returnValue.push_back(center.y + 1);
                    returnValue.push_back(center.z + 1);
                    i = 0;
                    back = true;
                }
            } else {
                if (i == 0) {
                    returnValue.push_back(center.x - 1);
                    returnValue.push_back(center.y - 1);
                    returnValue.push_back(center.z - 1);
                    i++;
                } else if (i == 1) {
                    returnValue.push_back(center.x + 1);
                    returnValue.push_back(center.y - 1);
                    returnValue.push_back(center.z - 1);
                    i++;
                } else if (i == 2) {
                    returnValue.push_back(center.x + 1);
                    returnValue.push_back(center.y + 1);
                    returnValue.push_back(center.z - 1);
                    i++;
                } else {
                    returnValue.push_back(center.x - 1);
                    returnValue.push_back(center.y + 1);
                    returnValue.push_back(center.z - 1);
                    i = 0;
                }
            }

            returnValue.push_back(vertices[j * stride + 3]);
            returnValue.push_back(vertices[j * stride + 4]);
        }
    }
}