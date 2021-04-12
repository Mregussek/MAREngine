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


#include "IRender.h"
#include "Shaders.h"


namespace marengine {


    struct FVertexInputVariableInfo {
        EInputType inputType{ EInputType::NONE };
        uint32_t location{ 0 };
        uint32_t offset{ 0 };
    };

    struct FVertexInputDescription {
        std::vector<FVertexInputVariableInfo> inputVariables;
        uint32_t binding{ 0 };
        uint32_t stride{ 0 };
    };


    class IBuffer : public FRenderResource {
    public:

        virtual void free() = 0;
        virtual void destroy() = 0;
        virtual void bind() = 0;

        virtual void update(const float* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const uint32_t* data, size_t offset, size_t sizeOfData) = 0;
        virtual void update(const int32_t* data, size_t offset, size_t sizeOfData) = 0;

    };


    class FBuffer : public IBuffer {
    public:

        virtual void create(int64_t memoryToAllocate) = 0;

    };


    class FVertexBuffer : public FBuffer {
    public:

        MAR_NO_DISCARD virtual const FVertexInputDescription& getInputDescription() const final;
        virtual void setInputDescription(const FVertexInputDescription& inputDescription) final;
        virtual void pushVariableInfo(const FVertexInputVariableInfo& info) final;

        virtual void update(const FVertexArray& vertices) = 0;

    protected:

        FVertexInputDescription p_inputDescription;
        int64_t p_allocatedMemory{ 0 };

    };


    class FIndexBuffer : public FBuffer {
    public:


        virtual void update(const FIndicesArray& indices) = 0;
        virtual void passIndicesCount(uint32_t indicesCount) final;
        MAR_NO_DISCARD virtual uint32_t getIndicesCount() const final;

    protected:

        int64_t p_allocatedMemory{ 0 };
        uint32_t p_indicesCount{ 0 };

    };


    class IShaderBuffer : public IBuffer {
    public:

        virtual void create() = 0;
        virtual void setInputDescription(const FShaderInputDescription& inputDescription) = 0;
        virtual void pushVariableInfo(const FShaderInputVariableInfo& info) = 0;
        virtual const FShaderInputDescription& getInputDescription() const = 0;

    };


    class FShaderBuffer : public IShaderBuffer {
    public:

        void setInputDescription(const FShaderInputDescription& inputDescription) final;
        void pushVariableInfo(const FShaderInputVariableInfo& info) final;
        MAR_NO_DISCARD const FShaderInputDescription& getInputDescription() const final;

        FShaderInputDescription p_inputDescription;

    };


    class FShaderStorageBuffer : public FShaderBuffer {

    };


    class FUniformBuffer : public FShaderBuffer {

    };


    class IBufferStorage : public IRenderResourceStorage {
    public:

        virtual size_t getCountSSBO() const = 0;
        virtual size_t getCountUBO() const = 0;
        virtual size_t getCountVBO() const = 0;
        virtual size_t getCountIBO() const = 0;

        virtual FShaderBuffer* getSSBO(int8_t index) const = 0;
        virtual FShaderBuffer* getUBO(int8_t index) const = 0;
        virtual FVertexBuffer* getVBO(int8_t index) const = 0;
        virtual FIndexBuffer* getIBO(int8_t index) const = 0;

    };


    class FBufferStorage : public IBufferStorage {

    };

    class RenderCamera;
    class FPointLightBatch;

    class IBufferFactory : public FRenderResourceFactory {
    public:

        virtual FShaderBuffer* emplaceSSBO() = 0;
        virtual FShaderBuffer* emplaceUBO() = 0;
        virtual FVertexBuffer* emplaceVBO() = 0;
        virtual FIndexBuffer* emplaceIBO() = 0;

        virtual uint32 fillCameraSSBO(FShaderBuffer* pShaderBuffer,
                                      const RenderCamera* const pRenderCamera) const = 0;
        virtual uint32 fillPointLightSSBO(FShaderBuffer* pShaderBuffer,
                                          const FPointLightBatch* const pPointLightBatch) const = 0;

    };


    class FBufferFactory : public IBufferFactory {
    public:

        MAR_NO_DISCARD uint32 fillCameraSSBO(FShaderBuffer* pShaderBuffer,
                                             const RenderCamera* const pRenderCamera) const final;
        MAR_NO_DISCARD uint32 fillPointLightSSBO(FShaderBuffer* pShaderBuffer,
                                                 const FPointLightBatch* const pPointLightBatch) const final;

    };


}


#endif //MARENGINE_IBUFFER_H
