/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MARENGINE_IBUFFER_H
#define MARENGINE_IBUFFER_H


#include "../Mesh/MeshDefinitions.h"


namespace marengine {


    enum class EBufferType {
        NONE, VERTEX, INDEX, SSBO, UBO
    };

    enum class EVertexInputType {
        NONE, FLOAT
    };

    struct FVertexLayoutElement {
        EVertexInputType inputType{ EVertexInputType::NONE };
        uint32_t count{ 0 };
    };

    typedef std::vector<FVertexLayoutElement> FVertexLayoutArray;


    class IBuffer {
    public:

        virtual void create(uint64_t memoryToAllocate, uint32_t bindingPoint) = 0;
        virtual void free() = 0;
        virtual void destroy() = 0;

        virtual void update(const float* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const uint32_t* data, size_t offset, size_t sizeOfData) = 0;

    };


    class FBuffer : public IBuffer {
    protected:

        void create(uint64_t memoryToAllocate, uint32_t bindingPoint) override;


        uint32_t p_bindingPoint{ 0 };
        uint64_t p_allocatedMemory{ 0 };

    };


    class FVertexBuffer : public FBuffer {
    public:

        virtual void passLayout(const FVertexLayoutArray& vertexLayout) final;
        virtual void update(const FVertexArray& vertices) = 0;

    protected:

        FVertexLayoutArray p_vertexLayoutArray;
        uint32_t p_stride{ 0 };
        static constexpr EBufferType p_bufferType{ EBufferType::VERTEX };

    };


    class FIndexBuffer : public FBuffer {
    public:

        virtual void update(const FIndicesArray& indices) = 0;

    protected:

        static constexpr EBufferType p_bufferType{ EBufferType::INDEX };

    };


    class FShaderStorageBuffer : public FBuffer {
    protected:

        static constexpr EBufferType p_bufferType{ EBufferType::SSBO };

    };


    class FUniformBuffer : public FBuffer {
    protected:

        static constexpr EBufferType p_bufferType{ EBufferType::UBO };

    };


}


#endif //MARENGINE_IBUFFER_H
