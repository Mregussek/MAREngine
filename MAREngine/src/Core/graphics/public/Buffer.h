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


#ifndef MARENGINE_FBUFFER_H
#define MARENGINE_FBUFFER_H


#include "IBuffer.h"
#include "Shaders.h"


namespace marengine {


    class FVertexBuffer : public IVertexBuffer {
    public:

        MAR_NO_DISCARD virtual const FVertexInputDescription& getInputDescription() const final;
        void setInputDescription(const FVertexInputDescription& inputDescription) final;
        void pushVariableInfo(const FVertexInputVariableInfo& info) final;

    protected:

        FVertexInputDescription p_inputDescription;
        int64_t p_allocatedMemory{ 0 };

    };


    class FIndexBuffer : public IIndexBuffer {
    public:

        void passIndicesCount(uint32 indicesCount) final;
        MAR_NO_DISCARD uint32 getIndicesCount() const final;

    protected:

        int64 p_allocatedMemory{ 0 };
        uint32 p_indicesCount{ 0 };

    };


    class FShaderBuffer : public IShaderBuffer {
    public:

        MAR_NO_DISCARD const FShaderInputDescription& getInputDescription() const final;
        void setInputDescription(const FShaderInputDescription& inputDescription) final;
        void pushVariableInfo(const FShaderInputVariableInfo& info) final;

    protected:

        FShaderInputDescription p_inputDescription;

    };


    class FBufferStorage : public IBufferStorage {

    };


    class FBufferFactory : public IBufferFactory {
    public:

        MAR_NO_DISCARD uint32 fillCameraSSBO(FShaderBuffer* const pShaderBuffer,
                                             const RenderCamera* const pRenderCamera) const final;
        MAR_NO_DISCARD uint32 fillPointLightSSBO(FShaderBuffer* const pShaderBuffer,
                                                 const FPointLightBatch* const pPointLightBatch) const final;

    };


}


#endif //MARENGINE_FBUFFER_H
