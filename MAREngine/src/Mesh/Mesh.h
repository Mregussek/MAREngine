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
        VertexBuffer vbo;
        VertexArray vao;
        ElementBuffer ebo;
        VertexBufferLayout lay;
        std::vector<float> _vertices;
        std::vector<unsigned int> _indices;

    public:
        Mesh(Cube& shape);
        Mesh(const std::vector<Cube>& shapes, const std::vector<glm::vec3>& centers);

        void bind();
        void unbind();

        const unsigned int sizeofVertices() const { return vbo.getSize(); }
        const unsigned int sizeofIndices() const { return ebo.getIndicesNumber(); }

        static void changeIndicesOfObject(const unsigned int* indices, const unsigned int& size, 
            const unsigned int& stride, std::vector<unsigned int>& returnValue, const int& indexOfObject);
        static void changeCenterOfObject(const float* vertices, const unsigned int& size, const unsigned int& stride,
            const glm::vec3& center, std::vector<float>& returnValue);
    };
}

#endif // !MESH_H