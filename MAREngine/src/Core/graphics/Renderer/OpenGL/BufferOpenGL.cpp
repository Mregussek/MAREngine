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


    void FBufferOpenGL::createGL(uint32_t bufferType, uint64_t memoryToAllocate) {
        PLATFORM_GL_FUNC( glGenBuffers(1, &p_id) );
        PLATFORM_GL_FUNC( glBindBuffer(bufferType, p_id) );
        PLATFORM_GL_FUNC( glBufferData(bufferType, memoryToAllocate, nullptr, GL_DYNAMIC_DRAW) );
    }

    void FBufferOpenGL::freeGL(uint32_t bufferType) const {
        bindGL(bufferType);
        PLATFORM_GL_FUNC( glBufferSubData(bufferType, 0, 0, nullptr) );
    }

    void FBufferOpenGL::closeGL() {
        PLATFORM_GL_FUNC( glDeleteBuffers(1, &p_id) );
    }

    void FBufferOpenGL::bindGL(uint32_t bufferType) const {
        PLATFORM_GL_FUNC( glBindBuffer(bufferType, p_id) );
    }

    void FBufferOpenGL::unbindGL(uint32_t bufferType) const {
        PLATFORM_GL_FUNC( glBindBuffer(bufferType, 0) );
    }

    void FBufferOpenGL::updateGL(uint32_t bufferType, const float* data, size_t offset, size_t sizeOfData) const {
        bindGL(bufferType);
        PLATFORM_GL_FUNC( glBufferSubData(bufferType, offset, sizeOfData, data) );
    }

    void FBufferOpenGL::updateGL(uint32_t bufferType, const uint32_t* data, size_t offset, size_t sizeOfData) const {
        bindGL(bufferType);
        PLATFORM_GL_FUNC( glBufferSubData(bufferType, offset, sizeOfData, data) );
    }



    void FVertexBufferOpenGL::create(uint64_t memoryToAllocate, uint32_t bindingPoint) {
        FBuffer::create(memoryToAllocate, bindingPoint);
        FBufferOpenGL::createGL(m_glBufferType, p_allocatedMemory);
    }

    void FVertexBufferOpenGL::free() {
        FBufferOpenGL::freeGL(m_glBufferType);
    }

    void FVertexBufferOpenGL::destroy() {
        FBufferOpenGL::closeGL();
    }

    void FVertexBufferOpenGL::update(const FVertexArray& vertices) {
        FBufferOpenGL::updateGL(m_glBufferType, &vertices[0].position.x, 0, vertices.size() * sizeof(vertices[0]) );
    }

    void FVertexBufferOpenGL::update(const float* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }

    void FVertexBufferOpenGL::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }



    void FIndexBufferOpenGL::create(uint64_t memoryToAllocate, uint32_t bindingPoint) {
        FBuffer::create(memoryToAllocate, bindingPoint);
        FBufferOpenGL::createGL(m_glBufferType, p_allocatedMemory);
    }

    void FIndexBufferOpenGL::free() {
        FBufferOpenGL::freeGL(m_glBufferType);
    }

    void FIndexBufferOpenGL::destroy() {
        FBufferOpenGL::closeGL();
    }

    void FIndexBufferOpenGL::update(const FIndicesArray& indices) {
        FBufferOpenGL::updateGL(m_glBufferType, indices.data(), 0, indices.size() * sizeof(indices[0]));
    }

    void FIndexBufferOpenGL::update(const float* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }

    void FIndexBufferOpenGL::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }



    void FShaderStorageBufferOpenGL2::create(uint64_t memoryToAllocate, uint32_t bindingPoint) {
        FBuffer::create(memoryToAllocate, bindingPoint);
        FBufferOpenGL::createGL(m_glBufferType, p_allocatedMemory);
        PLATFORM_GL_FUNC( glBindBufferBase(m_glBufferType, p_bindingPoint, p_id) );
    }

    void FShaderStorageBufferOpenGL2::free() {
        FBufferOpenGL::freeGL(m_glBufferType);
    }

    void FShaderStorageBufferOpenGL2::destroy() {
        FBufferOpenGL::closeGL();
    }

    void FShaderStorageBufferOpenGL2::update(const float* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }

    void FShaderStorageBufferOpenGL2::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }



    void FUniformBufferOpenGL2::create(uint64_t memoryToAllocate, uint32_t bindingPoint) {
        FBuffer::create(memoryToAllocate, bindingPoint);
        FBufferOpenGL::createGL(m_glBufferType, p_allocatedMemory);
        PLATFORM_GL_FUNC( glBindBufferBase(m_glBufferType, p_bindingPoint, p_id) );
    }

    void FUniformBufferOpenGL2::free() {
        FBufferOpenGL::freeGL(m_glBufferType);
    }

    void FUniformBufferOpenGL2::destroy() {
        FBufferOpenGL::closeGL();
    }

    void FUniformBufferOpenGL2::update(const float* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }

    void FUniformBufferOpenGL2::update(const uint32_t* data, size_t offset, size_t sizeOfData) {
        FBufferOpenGL::updateGL(m_glBufferType, data, offset, sizeOfData);
    }



}
