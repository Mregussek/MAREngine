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


    class FBuffer : public IBuffer {
    protected:

        void create(int64_t memoryToAllocate, uint32_t bindingPoint) override;


        uint32_t p_bindingPoint{ 0 };
        int64_t p_allocatedMemory{ 0 };

    };


    class FVertexBuffer : public FBuffer {
    public:

        virtual void passLayout(const FVertexLayoutArray& vertexLayout) final;
        virtual void update(const FVertexArray& vertices) = 0;

    protected:

        FVertexLayoutArray p_vertexLayoutArray;
        uint32_t p_stride{ 0 };

    };


    class FIndexBuffer : public FBuffer {
    public:

        virtual void update(const FIndicesArray& indices) = 0;

    };


    class FShaderStorageBuffer : public FBuffer {
    public:


    };


    class FUniformBuffer : public FBuffer {
    public:


    };


}


#endif //MARENGINE_IBUFFER_H
