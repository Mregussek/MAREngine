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

namespace mar {
    class Mesh {
        VertexBuffer vbo;
        VertexArray vao;
        ElementBuffer ebo;
        VertexBufferLayout lay;

    public:
        Mesh(Cube* shape);

        void bind();
        void unbind();

        const unsigned int sizeofVertices() const { return vbo.getSize(); }
        const unsigned int sizeofIndices() const { return ebo.getIndicesNumber(); }
    };
}

#endif // !MESH_H