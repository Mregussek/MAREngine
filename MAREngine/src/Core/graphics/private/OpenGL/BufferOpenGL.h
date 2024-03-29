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


#include "../../public/Buffer.h"


namespace marengine {


    class FVertexBufferOpenGL : public FVertexBuffer {
    public:

        void create(int64 memoryToAllocate) final;
        void free() final;
        void destroy() final;

        void bind() const final;
        void update(const FVertexArray& vertices) const final;
        void update(const float* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const int32* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const uint32* data, uint32 offset, uint32 sizeOfData) const final;

    private:

        static constexpr GLenum m_glBufferType{ GL_ARRAY_BUFFER };
        uint32 m_id{ 0 };

    };


    class FIndexBufferOpenGL : public FIndexBuffer {
    public:

        void create(int64 memoryToAllocate) final;
        void free() final;
        void destroy() final;

        void bind() const final;
        void update(const FIndicesArray& indices) const final;
        void update(const float* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const int32* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const uint32* data, uint32 offset, uint32 sizeOfData) const final;

    private:

        static constexpr GLenum m_glBufferType{ GL_ELEMENT_ARRAY_BUFFER };
        uint32 m_id{ 0 };

    };


    class FShaderStorageBufferOpenGL : public FShaderBuffer {
    public:

        void create() final;
        void free() final;
        void destroy() final;

        void bind() const final;
        void update(const float* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const int32* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const uint32* data, uint32 offset, uint32 sizeOfData) const final;

    private:

        static constexpr GLenum m_glBufferType{ GL_SHADER_STORAGE_BUFFER };
        uint32 m_id{ 0 };

    };


    class FUniformBufferOpenGL : public FShaderBuffer {
    public:

        void create() final;
        void free() final;
        void destroy() final;

        void bind() const final;
        void update(const float* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const int32* data, uint32 offset, uint32 sizeOfData) const final;
        void update(const uint32* data, uint32 offset, uint32 sizeOfData) const final;

    private:

        static constexpr GLenum m_glBufferType{ GL_UNIFORM_BUFFER };
        uint32 m_id{ 0 };

    };


    class FBufferStorageOpenGL : public FBufferStorage {

        friend class FBufferFactoryOpenGL;

    public:

        MAR_NO_DISCARD uint32 getCountSSBO() const final;
        MAR_NO_DISCARD uint32 getCountUBO() const final;
        MAR_NO_DISCARD uint32 getCountVBO() const final;
        MAR_NO_DISCARD uint32 getCountIBO() const final;

        MAR_NO_DISCARD FShaderBuffer* getSSBO(int32 index) const final;
        MAR_NO_DISCARD FShaderBuffer* getUBO(int32 index) const final;
        MAR_NO_DISCARD FVertexBuffer* getVBO(int32 index) const final;
        MAR_NO_DISCARD FIndexBuffer* getIBO(int32 index) const final;

        void reset() final;

    private:

        std::vector<FShaderStorageBufferOpenGL> m_ssbos;
        std::vector<FUniformBufferOpenGL> m_ubos;
        std::vector<FVertexBufferOpenGL> m_vbos;
        std::vector<FIndexBufferOpenGL> m_ibos;

    };


    class FBufferFactoryOpenGL : public FBufferFactory {

        friend class FRenderContextOpenGL;

    public:

        MAR_NO_DISCARD FShaderBuffer* emplaceSSBO() final;
        MAR_NO_DISCARD FShaderBuffer* emplaceUBO() final;
        MAR_NO_DISCARD FVertexBuffer* emplaceVBO() final;
        MAR_NO_DISCARD FIndexBuffer* emplaceIBO() final;

    private:

        FBufferStorageOpenGL m_storage;

    };


}



#endif //MARENGINE_BUFFEROPENGL_H
