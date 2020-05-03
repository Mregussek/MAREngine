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
        Mesh();
        Mesh(Cube& shape);
        ~Mesh();

        void initialize();
        void bind();
        void unbind();

        void push(Cube& cube, glm::vec3 position);

        const unsigned int sizeofVertices() const { return _vbo.getSize(); }
        const unsigned int sizeofIndices() const { return _ebo.getIndicesNumber(); }

        static void extendID(Cube& cube, const float& nextID);

        static void changeCenterOfObject(Cube& cube, const glm::vec3& center);
        static std::vector<float> changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
                    const glm::vec3& center, std::vector<float>& returnValue);

        static void changeIndicesFormat(Cube& cube, unsigned int& max_value);
        static void changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                    std::vector<unsigned int>& returnValue);
    };
}

#endif // !MESH_H