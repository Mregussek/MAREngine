/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Mesh.h"

namespace mar {
    Mesh::Mesh(Cube* shape)
        : vao(VertexArray()),
        vbo(VertexBuffer(shape->getSizeofVertices(), shape->vertices)),
        ebo(ElementBuffer(shape->getSizeofIndices(), shape->indices)),
        lay(VertexBufferLayout())
    {

        for (auto const& l : shape->layout) lay.push<float>(l);

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
}