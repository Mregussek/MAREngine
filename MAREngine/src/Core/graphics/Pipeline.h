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


#ifndef MARENGINE_PIPELINE_H
#define MARENGINE_PIPELINE_H


#include "IRender.h"


namespace marengine {

    class FShadersStorage;
    class FBufferStorage;


    class IPipeline : public FRenderResource {
    public:

        virtual void passBufferStorage(FBufferStorage* pBufferStorage) = 0;
        virtual void passShadersStorage(FShadersStorage* pShadersStorage) = 0;

        virtual void create() = 0;
        virtual void close() = 0;
        virtual void bind() = 0;

    };


    class FPipeline : public IPipeline {
    public:

        virtual void passBufferStorage(FBufferStorage* pBufferStorage) final;
        virtual void passShadersStorage(FShadersStorage* pShadersStorage) final;

    protected:

        FBufferStorage* p_pBufferStorage{ nullptr };
        FShadersStorage* p_pShadersStorage{ nullptr };

    };


    class FPipelineMesh : public FPipeline {
    public:

        virtual void passIndexBuffer(int8_t i) final;
        virtual void passCameraSSBO(int8_t i) final;
        virtual void passPointLightSSBO(int8_t i) final;
        MAR_NO_DISCARD virtual uint32_t getIndicesCount() const final;

    protected:

        int8_t p_iboIndex{ -1 };
        int8_t p_camIndex{ -1 };
        int8_t p_pointLightIndex{ -1 };

    };


    class FPipelineMeshColor : public FPipelineMesh {
    public:

        virtual void passVertexBuffer(int8_t i) final;
        virtual void passTransformSSBO(int8_t i) final;
        virtual void passColorSSBO(int8_t i) final;
        virtual void passShaderPipeline(int8_t i) final;

    protected:

        int8_t p_vboIndex{ -1 };
        int8_t p_transformIndex{ -1 };
        int8_t p_colorIndex{ -1 };
        int8_t p_shaderIndex{ -1 };

    };


    class FPipelineMeshTex2D : public FPipelineMesh {

    };


    class IPipelineStorage : public IRenderResourceStorage {
    public:

        virtual FPipelineMeshColor* getColorMesh(int8_t index) const = 0;
        virtual FPipelineMeshTex2D* getTex2DMesh(int8_t index) const = 0;

        virtual size_t getCountColorMesh() const = 0;
        virtual size_t getCountTex2DMesh() const = 0;

    };


    class FPipelineStorage : public IPipelineStorage {

    };


    class IPipelineFactory : public IRenderResourceFactory {
    public:

        virtual FPipelineMeshColor* emplaceColorMesh() = 0;
        virtual FPipelineMeshTex2D* emplaceTex2DMesh() = 0;

    };


    class FPipelineFactory : public IPipelineFactory {

    };








}


#endif //MARENGINE_IGRAPHICSPIPELINE_H