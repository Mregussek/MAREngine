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
        Mesh(Cube& shape) 
            : vao(VertexArray()),
            vbo(VertexBuffer(shape.getSizeofVertices(), shape.vertices)),
            ebo(ElementBuffer(shape.getSizeofIndices(), shape.indices)),
            lay(VertexBufferLayout())
        {
            
            for (auto const& l : shape.layout) lay.push<float>(l);

            vao.addBuffer(vbo, lay);
        }

        void bind() {
            vao.bind();
            ebo.bind();
        }

        void unbind() {
            vao.unbind();
            vbo.unbind();
            ebo.unbind();
        }

        const unsigned int sizeofVertices() const { return vbo.getSize(); }
        const unsigned int sizeofIndices() const { return ebo.getIndicesNumber(); }
    };
}

#endif // !MESH_H