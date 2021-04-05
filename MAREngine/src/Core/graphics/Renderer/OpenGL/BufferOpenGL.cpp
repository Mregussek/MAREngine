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


#include "BufferOpenGL.h"
#include "../../../../Logging/Logger.h"


namespace marengine {


    template<GLenum TBufferType>
    static void bindGL(uint32_t id) {
        GL_FUNC( glBindBuffer(TBufferType, id) );
    }

    template<GLenum TBufferType>
    static void unbindGL() {
        GL_FUNC( glBindBuffer(TBufferType, 0) );
    }

    template<GLenum TBufferType>
    static void createGL(uint32_t& id, int64_t memoryToAllocate) {
        GL_FUNC( glGenBuffers(1, &id) );
        GL_FUNC( glBindBuffer(TBufferType, id) );
        GL_FUNC( glBufferData(TBufferType, (long)memoryToAllocate, nullptr, GL_DYNAMIC_DRAW) );
    }

    template<GLenum TBufferType>
    static void freeGL(uint32_t id) {
        bindGL<TBufferType>(id);
        GL_FUNC( glBufferSubData(TBufferType, 0, 0, nullptr) );
    }

    static void closeGL(uint32_t& id) {
        GL_FUNC( glDeleteBuffers(1, &id) );
    }

    template<GLenum TBufferType>
    static void updateGL(const float* data, size_t offset, size_t sizeOfData) {
        GL_FUNC( glBufferSubData(TBufferType, offset, sizeOfData, data) );
    }

    template<GLenum TBufferType>
    static void updateGL(const uint32_t* data, size_t offset, size_t sizeOfData) {
        GL_FUNC( glBufferSubData(TBufferType, offset, sizeOfData, data) );
    }

    static int64_t getMemoryUsed(FShaderBufferItemsArray& itemsArray) {
        int64_t sum = 0;
        for(const auto& item : itemsArray) {
            sum += item.memoryUsed;
        }
        return sum;
    }



    void FVertexBufferOpenGL::create(int64_t memoryToAllocate) {
        p_allocatedMemory = memoryToAllocate;
        createGL<m_glBufferType>(m_id, p_allocatedMemory);
    }

    void FVertexBufferOpenGL::free() {
        freeGL<m_glBufferType>(m_id);
    }

    void FVertexBufferOpenGL::destroy() {
        closeGL(m_id);
    }

    void FVertexBufferOpenGL::bind() {
        bindGL<m_glBufferType>(m_id);
    }

    void FVertexBufferOpenGL::update(const FVertexArray& vertices) {
        bind();
        updateGL<m_glBufferType>(&vertices[0].position.x, 0, vertices.size() * sizeof(vertices[0]) );
    }

    void FVertexBufferOpenGL::update(const float* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }

    void FVertexBufferOpenGL::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }



    void FIndexBufferOpenGL::create(int64_t memoryToAllocate) {
        p_allocatedMemory = memoryToAllocate;
        createGL<m_glBufferType>(m_id, p_allocatedMemory);
    }

    void FIndexBufferOpenGL::free() {
        freeGL<m_glBufferType>(m_id);
    }

    void FIndexBufferOpenGL::destroy() {
        closeGL(m_id);
    }

    void FIndexBufferOpenGL::bind() {
        bindGL<m_glBufferType>(m_id);
    }

    void FIndexBufferOpenGL::update(const FIndicesArray& indices) {
        bind();
        updateGL<m_glBufferType>(indices.data(), 0, indices.size() * sizeof(indices[0]));
    }

    void FIndexBufferOpenGL::update(const float* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }

    void FIndexBufferOpenGL::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }



    void FShaderStorageBufferOpenGL2::create() {
        const int64_t memoryUsed{ getMemoryUsed(p_inputLayoutInfo.items) };
        createGL<m_glBufferType>(m_id, memoryUsed);
        GL_FUNC( glBindBufferBase(m_glBufferType, p_inputLayoutInfo.binding, m_id) );
    }

    void FShaderStorageBufferOpenGL2::free() {
        freeGL<m_glBufferType>(m_id);
    }

    void FShaderStorageBufferOpenGL2::destroy() {
        closeGL(m_id);
    }

    void FShaderStorageBufferOpenGL2::bind() {
        bindGL<m_glBufferType>(m_id);
    }

    void FShaderStorageBufferOpenGL2::update(const float* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }

    void FShaderStorageBufferOpenGL2::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }



    void FUniformBufferOpenGL2::create() {
        const int64_t memoryUsed{ getMemoryUsed(p_inputLayoutInfo.items) };
        createGL<m_glBufferType>(m_id, memoryUsed);
        GL_FUNC( glBindBufferBase(m_glBufferType, p_inputLayoutInfo.binding, m_id) );
    }

    void FUniformBufferOpenGL2::free() {
        freeGL<m_glBufferType>(m_id);
    }

    void FUniformBufferOpenGL2::destroy() {
        closeGL(m_id);
    }

    void FUniformBufferOpenGL2::bind() {
        bindGL<m_glBufferType>(m_id);
    }

    void FUniformBufferOpenGL2::update(const float* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }

    void FUniformBufferOpenGL2::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        bind();
        updateGL<m_glBufferType>(data, offset, sizeOfData);
    }



}
