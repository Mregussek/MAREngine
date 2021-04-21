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


#include "IRenderResource.h"


namespace marengine {

    class RenderCamera;
    class FPointLightBatch;
    class FShaderBuffer;
    class FVertexBuffer;
    class FIndexBuffer;
    struct FShaderInputDescription;
    struct FShaderInputVariableInfo;


    struct FVertexInputVariableInfo {
        EInputType inputType{ EInputType::NONE };
        uint32 location{ 0 };
        uint32 offset{ 0 };
    };

    struct FVertexInputDescription {
        std::vector<FVertexInputVariableInfo> inputVariables;
        uint32 binding{ 0 };
        uint32 stride{ 0 };
    };


    class IBuffer : public FRenderResource {
    public:

        virtual void free() = 0;
        virtual void destroy() = 0;
        virtual void bind() const = 0;

        virtual void update(const float* data, uint32 offset, uint32 sizeOfData) const = 0;
        virtual void update(const uint32* data, uint32 offset, uint32 sizeOfData) const = 0;
        virtual void update(const int32* data, uint32 offset, uint32 sizeOfData) const = 0;

    };


    class IMeshBuffer : public IBuffer {
    public:

        virtual void create(int64 memoryToAllocate) = 0;

    };


    class IVertexBuffer : public IMeshBuffer {
    public:

        virtual const FVertexInputDescription& getInputDescription() const = 0;
        virtual void setInputDescription(const FVertexInputDescription& inputDescription) = 0;
        virtual void pushVariableInfo(const FVertexInputVariableInfo& info) = 0;

        virtual void update(const FVertexArray& vertices) const = 0;

    };


    class IIndexBuffer : public IMeshBuffer {
    public:

        virtual void passIndicesCount(uint32 indicesCount) = 0;
        virtual uint32 getIndicesCount() const = 0;

        virtual void update(const FIndicesArray& indices) const = 0;

    };


    class IShaderBuffer : public IBuffer {
    public:

        virtual void create() = 0;

        virtual const FShaderInputDescription& getInputDescription() const = 0;
        virtual void setInputDescription(const FShaderInputDescription& inputDescription) = 0;
        virtual void pushVariableInfo(const FShaderInputVariableInfo& info) = 0;

    };


    class IBufferStorage : public IRenderResourceStorage {
    public:

        virtual uint32 getCountSSBO() const = 0;
        virtual uint32 getCountUBO() const = 0;
        virtual uint32 getCountVBO() const = 0;
        virtual uint32 getCountIBO() const = 0;

        virtual FShaderBuffer* getSSBO(int32 index) const = 0;
        virtual FShaderBuffer* getUBO(int32 index) const = 0;
        virtual FVertexBuffer* getVBO(int32 index) const = 0;
        virtual FIndexBuffer* getIBO(int32 index) const = 0;

    };


    class IBufferFactory : public FRenderResourceFactory {
    public:

        virtual FShaderBuffer* emplaceSSBO() = 0;
        virtual FShaderBuffer* emplaceUBO() = 0;
        virtual FVertexBuffer* emplaceVBO() = 0;
        virtual FIndexBuffer* emplaceIBO() = 0;

        virtual uint32 fillCameraSSBO(FShaderBuffer* const pShaderBuffer,
                                      const RenderCamera* const pRenderCamera) const = 0;
        virtual uint32 fillPointLightSSBO(FShaderBuffer* const pShaderBuffer,
                                          const FPointLightBatch* const pPointLightBatch) const = 0;

    };



}


#endif //MARENGINE_IBUFFER_H
