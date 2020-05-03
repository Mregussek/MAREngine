/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    Mesh::Mesh()
        : _vao(VertexArray()),
        lay(VertexBufferLayout()),
        _pushedOnce(false),
        _maxValue(0)
    { }

    Mesh::Mesh(Cube& shape)
        : _vao(VertexArray()),
        _vbo(VertexBuffer(shape.verticesVector)),
        _ebo(ElementBuffer(shape.indicesVector)),
        lay(VertexBufferLayout()),
        _pushedOnce(false),
        _maxValue(0)
    {
        for (auto const& l : shape.layout) lay.push<float>(l);

        _vao.addBuffer(_vbo, lay);
    }

    Mesh::~Mesh() {
        _vbo.close();
        _ebo.close();
    }

    void Mesh::initialize() {
        _vbo = VertexBuffer(_vertices);
        _ebo = ElementBuffer(_indices);

        _vao.addBuffer(_vbo, lay);
    }

    void Mesh::push(Cube& cube, glm::vec3 position) {
        _shapes.emplace_back(cube);

        changeCenterOfObject(cube, position);
        extendID(cube, _shapes.size() - 1);
        _vertices.insert(_vertices.end(), cube.verticesVector.begin(), cube.verticesVector.end());

        changeIndicesFormat(cube, _maxValue);
        _maxValue += cube.getSizeofVertices() / cube.getStride();

        _indices.insert(_indices.end(), cube.indicesVector.begin(), cube.indicesVector.end());

        if (!_pushedOnce) {
            for (auto const& l : cube.layout) lay.push<float>(l);
            _pushedOnce = true;
        }
    }

    void Mesh::bind() {
        _vao.bind();
        _ebo.bind();
    }

    void Mesh::unbind() {
        _vao.unbind();
        _vbo.unbind();
        _ebo.unbind();
    }

    void Mesh::extendID(Cube& cube, const float& nextID) {
        unsigned int size = cube.getSizeofVertices();
        unsigned int stride = cube.getStride();

        for (unsigned int j = 1; j < size / stride; j++) 
            cube.verticesVector[j * stride - 1] = nextID;
    }

    void Mesh::changeCenterOfObject(Cube& cube, const glm::vec3& center) {
        cube.verticesVector = changeCenterOfObject(cube.getSizeofVertices(), cube.getStride(), center, cube.verticesVector);
        cube.prescribeCenter(center);
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

    void Mesh::changeIndicesFormat(Cube& cube, unsigned int& max_value) {
        changeIndicesFormat(cube.getSizeofIndices(), max_value, cube.indicesVector);
    }

    void Mesh::changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                     std::vector<unsigned int>& returnValue) {
        
            for (unsigned int i = 0; i < size; i++)
                returnValue[i] += max_value;
    }
}