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
#include "Texture/Texture.h"
#include "Shapes/Shapes.h"
#include "Shapes/Cube.h"

namespace mar {
    class Mesh {
        // --- Buffers --- //
        VertexBuffer _vbo;
        VertexArray _vao;
        ElementBuffer _ebo;
        VertexBufferLayout _lay;
        Texture _texture;
        // --- Global vectors --- //
        std::vector<Cube> _shapes;
        std::vector<float> _vertices;
        std::vector<unsigned int> _indices;
        // --- Helper for proper working --- //
        bool _pushedOnce;           // checks if layout is pushed
        unsigned int _maxValue;     // current max value of indices vector

    public:
        Mesh();
        Mesh(Cube& shape);
        ~Mesh();

        void initialize();
        void initializeBatch();
        void onUpdate(std::vector<glm::vec3> newCenters);
        void bind();
        void unbind();

        void push(Cube* cube, glm::vec3& position, std::string& texturePath);

        const unsigned int sizeofVertices() const { return _vbo.getSize(); }
        const unsigned int sizeofIndices() const { return _ebo.getIndicesNumber(); }

        static void extendID(Cube* cube, const float& nextID);

        static void changeCenterOfObject(Cube* cube, const glm::vec3& center);
        static std::vector<float> changeCenterOfObject(const unsigned int& size, const unsigned int& stride,
                    const glm::vec3& center, std::vector<float>& returnValue);

        static void changeIndicesFormat(Cube* cube, unsigned int& max_value);
        static void changeIndicesFormat(const unsigned int& size, unsigned int& max_value,
                    std::vector<unsigned int>& returnValue);
    };
}

#endif // !MESH_H