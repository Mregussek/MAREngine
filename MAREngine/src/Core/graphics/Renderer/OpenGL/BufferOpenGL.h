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


#ifndef MARENGINE_BUFFEROPENGL_H
#define MARENGINE_BUFFEROPENGL_H


#include "../IBuffer.h"


namespace marengine {


    class FBufferOpenGL {
    protected:

        void createGL(uint32_t bufferType, uint64_t memoryToAllocate);
        void freeGL(uint32_t bufferType) const;
        void closeGL();

        void bindGL(uint32_t bufferType) const;
        void unbindGL(uint32_t bufferType) const;

        void updateGL(uint32_t bufferType, const float* data, size_t offset, size_t sizeOfData) const;
        void updateGL(uint32_t bufferType, const uint32_t* data, size_t offset, size_t sizeOfData) const;

        uint32_t p_id{ 0 };

    };


    class FVertexBufferOpenGL : public FVertexBuffer, FBufferOpenGL {
    public:

        void create(uint64_t memoryToAllocate, uint32_t bindingPoint) final;
        void free() final;
        void destroy() final;

        void update(const FVertexArray& vertices) final;
        void update(const float* data, size_t offset, size_t sizeOfData) final;
        void update(const uint32_t* data, size_t offset, size_t sizeOfData) final;

    private:

        static constexpr uint32_t m_glBufferType{ GL_ARRAY_BUFFER };

    };


    class FIndexBufferOpenGL : public FIndexBuffer, FBufferOpenGL {
    public:

        void create(uint64_t memoryToAllocate, uint32_t bindingPoint) final;
        void free() final;
        void destroy() final;

        void update(const FIndicesArray& indices) final;
        void update(const float* data, size_t offset, size_t sizeOfData) final;
        void update(const uint32_t* data, size_t offset, size_t sizeOfData) final;

    private:

        static constexpr uint32_t m_glBufferType{ GL_ELEMENT_ARRAY_BUFFER };

    };


    class FShaderStorageBufferOpenGL2 : public FShaderStorageBuffer, FBufferOpenGL {
    public:

        void create(uint64_t memoryToAllocate, uint32_t bindingPoint) final;
        void free() final;
        void destroy() final;

        void update(const float* data, size_t offset, size_t sizeOfData) final;
        void update(const uint32_t* data, size_t offset, size_t sizeOfData) final;

    private:

        static constexpr uint32_t m_glBufferType{ GL_SHADER_STORAGE_BUFFER };

    };


    class FUniformBufferOpenGL2 : public FUniformBuffer, FBufferOpenGL {
    public:

        void create(uint64_t memoryToAllocate, uint32_t bindingPoint) final;
        void free() final;
        void destroy() final;

        void update(const float* data, size_t offset, size_t sizeOfData) final;
        void update(const uint32_t* data, size_t offset, size_t sizeOfData) final;

    private:

        static constexpr uint32_t m_glBufferType{ GL_UNIFORM_BUFFER };

    };


}



#endif //MARENGINE_BUFFEROPENGL_H
