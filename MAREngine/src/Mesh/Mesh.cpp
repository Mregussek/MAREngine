/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {

    Mesh::Mesh()
        : _vao(VertexArray()),
        _lay(VertexBufferLayout()),
        _texture(Texture()),
        _pushedOnce(false),
        _maxValue(0)
    { 
        // Constructor, if we use batch rendering
        // we need to push objects to make it proper
    }

    Mesh::Mesh(Cube& shape)
        : _vao(VertexArray()),
        _vbo(VertexBuffer(shape.verticesVector)),
        _ebo(ElementBuffer(shape.indicesVector, shape.indicesVector.size() * sizeof(unsigned int))),
        _lay(VertexBufferLayout()),
        _pushedOnce(false),
        _maxValue(0)
    {
        // constructor for infinite draw calls
        for (auto const& l : shape.layout) 
            _lay.push<float>(l);

        _vao.addBuffer(_vbo, _lay);
    }

    Mesh::~Mesh() {
        // Close Buffers
        _vbo.close();
        _ebo.close();
    }

    void Mesh::initialize() {
        _vbo = VertexBuffer(constants::maxCubeCount);
        _ebo = ElementBuffer(_indices, constants::maxIndexCount);

        _vao.addBufferBatch(_vbo, _lay);

        for (unsigned int i = 0; i < _shapes.size(); i++)
            _texture.bind(_shapes[i].getID(), _texture.getID(i));
    }

    void Mesh::onUpdate(std::vector<glm::vec3> newCenters) {
        _vbo.bind(); // set dynamic vertex buffer
        _vertices.clear(); // we are gonna put here new vertices

        for (unsigned int i = 0; i < _shapes.size(); i++) {
            changeCenterOfObject(&_shapes[i], newCenters[i]);
            _vertices.insert(_vertices.end(), _shapes[i].verticesVector.begin(), _shapes[i].verticesVector.end());
        }
        
        _vbo.updateDynamically(_vertices); // end _vertices, which are rendered
    }

    void Mesh::push(Cube* cube, glm::vec3& position,
        std::string& texturePath) {
        // cube - object, which we want to push
        // position - center pos, where we want object to be placed
        // texturePath - path to texture, which we want to use on this object

        if (cube->indicesVector.size() + _indices.size() > constants::maxIndexCount) {
            std::cout << "Cannot insert more indices!!!\n";
            return;
        }
        else if(cube->verticesVector.size() + _vertices.size() > constants::maxVertexCount) {
            std::cout << "Cannot insert more vertices!!!\n";
            return;
        }

        extendID(cube, (float)_shapes.size()); // more objects, more texture indexes

        changeCenterOfObject(cube, position); // user sends new center position, we need to change vertices

        _vertices.insert(_vertices.end(), cube->verticesVector.begin(), cube->verticesVector.end()); // insert object vertices to mesh vertices (batch rendering)

        changeIndicesFormat(cube, _maxValue); // we cannot use the same indices for the another vertices, that's why we increase them

        _maxValue += cube->getSizeofVertices() / cube->getStride(); // maximum value of indices
        
        _indices.insert(_indices.end(), cube->indicesVector.begin(), cube->indicesVector.end()); // insert object indices to mesh indices (batch rendering) 

        _shapes.emplace_back(*cube); // place new shape at the end of vector

        _texture.loadTexture(texturePath); // load texture for this object

        if (!_pushedOnce) { // push layout, all objects has the same format, so we need to do it once
            for (auto const& l : cube->layout) 
                _lay.push<float>(l);

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

    void Mesh::extendID(Cube* cube, const float& nextID) {
        unsigned int size = cube->getSizeofVertices();
        unsigned int stride = cube->getStride();

        // extend all vertices, which defines texture id
        for (unsigned int j = 1; j < size / stride; j++)
            cube->changeVerticesIndex(j * stride - 1, nextID);

        cube->setID(nextID);
    }

    void Mesh::changeCenterOfObject(Cube* cube, const glm::vec3& center) {
        // change vertices of this specific object
        cube->verticesVector = changeCenterOfObject(cube->getSizeofVertices(), cube->getStride(), center, cube->verticesVector);
        // change its center
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