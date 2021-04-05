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


    class FBuffer : public IBuffer { };

    
    class IShaderBuffer : public FBuffer {
    public:

        virtual void create() = 0;
        virtual void pushItem(const FShaderBufferItem& item) = 0;
        virtual const FShaderInputLayoutInfo& getInputLayoutInfo() const = 0;
        virtual void setInputLayoutInfo(const FShaderInputLayoutInfo& inputLayout) = 0;

    };
    
    
    class FShaderBuffer : public IShaderBuffer {
    public:

        void pushItem(const FShaderBufferItem& item) final;
        const FShaderInputLayoutInfo& getInputLayoutInfo() const final;
        void setInputLayoutInfo(const FShaderInputLayoutInfo& inputLayout) final;

    protected:

        FShaderInputLayoutInfo p_inputLayoutInfo;

    };


    class FVertexBuffer : public FBuffer {
    public:

        virtual void create(int64_t memoryToAllocate) = 0;
        virtual void pushInputElement(const FVertexInputLayoutInfo& inputLayout) final;
        virtual const FVertexInputDescription& getInputDescription() const final;
        virtual void setInputDescription(const FVertexInputDescription& inputDescription) final;

        virtual void update(const FVertexArray& vertices) = 0;

    protected:

        FVertexInputDescription p_inputDescription;
        int64_t p_allocatedMemory{ 0 };

    };


    class FIndexBuffer : public FBuffer {
    public:

        virtual void create(int64_t memoryToAllocate) = 0;
        virtual void update(const FIndicesArray& indices) = 0;
        virtual void passIndicesCount(uint32_t indicesCount) final;
        virtual uint32_t getIndicesCount() const final;

    protected:

        int64_t p_allocatedMemory{ 0 };
        uint32_t p_indicesCount{ 0 };

    };


    class FShaderStorageBuffer : public FShaderBuffer {

    };


    class FUniformBuffer : public FShaderBuffer {

    };


}


#endif //MARENGINE_IBUFFER_H
