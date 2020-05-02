/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MESH_H
#define MESH_H

#include "../mar.h"
#include "VertexBuffer/VertexBuffer.h"
#include "VertexBuffer/VertexBufferLayout.h"
#include "ElementBuffer/ElementBuffer.h"
#include "VertexArray/VertexArray.h"
#include "Shapes/Shapes.h"
#include "Shapes/Cube.h"

namespace mar {
    class Mesh {
        VertexBuffer _vbo;
        VertexArray _vao;
        ElementBuffer _ebo;
        VertexBufferLayout lay;
        std::vector<Cube> _shapes;
        std::vector<float> _vertices;
        std::vector<unsigned int> _indices;
        bool _pushedOnce;
        unsigned int _maxValue;

    public:
        Mesh() 
            : _vao(VertexArray()),
            lay(VertexBufferLayout()),
            _pushedOnce(false),
            _maxValue(0)
        { }

        Mesh(Cube& shape);

        ~Mesh() {
            _vbo.close();
            _ebo.close();
        }

        void initialize() {
            _vbo = VertexBuffer(_vertices);
            _ebo = ElementBuffer(_indices);

            _vao.addBuffer(_vbo, lay);
        }

        void bind();
        void unbind();

        void push(Cube& cube) {
            _shapes.emplace_back(cube);
            _vertices.insert(_vertices.end(), cube.verticesVector.begin(), cube.verticesVector.end());

            changeIndicesFormat(cube, _maxValue);
            _maxValue += 8;
            
            _indices.insert(_indices.end(), cube.indicesVector.begin(), cube.indicesVector.end());
        
            if (!_pushedOnce) {
                for (auto const& l : cube.layout) lay.push<float>(l);
                _pushedOnce = true;
            }
        }

        const unsigned int sizeofVertices() const { return _vbo.getSize(); }
        const unsigned int sizeofIndices() const { return _ebo.getIndicesNumber(); }

        static void changeCenterOfObject(Cube& cube, const glm::vec3& center);
        static void changeCenterOfObject(const float* vertices, const unsigned int& size, const unsigned int& stride,
            const glm::vec3& center, std::vector<float>& returnValue);

        static void changeIndicesFormat(Cube& cube, unsigned int& max_value);
        static void changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
            std::vector<unsigned int>& returnValue);
    };
}

#endif // !MESH_H