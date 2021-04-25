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


#ifndef MARENGINE_FPIPELINE_H
#define MARENGINE_FPIPELINE_H


#include "IPipeline.h"


namespace marengine {

    class FShadersStorage;
    class FBufferStorage;
    class FMaterialStorage;
    class FMeshBatchStaticColor;


    class FPipeline : public IPipeline {
    public:

        void passBufferStorage(FBufferStorage* pBufferStorage) final;
        void passShadersStorage(FShadersStorage* pShadersStorage) final;
        void passMaterialStorage(FMaterialStorage* pMaterialStorage) final;

    protected:

        FBufferStorage* p_pBufferStorage{ nullptr };
        FShadersStorage* p_pShadersStorage{ nullptr };
        FMaterialStorage* p_pMaterialStorage{ nullptr };

    };


    class FPipelineMesh : public FPipeline {
    public:

        virtual void passVertexBuffer(int32 i) final;
        virtual void passIndexBuffer(int32 i) final;
        virtual void passTransformSSBO(int32 i) final;
        virtual void passCameraSSBO(int32 i) final;
        virtual void passPointLightSSBO(int32 i) final;
        virtual void passShaderPipeline(int32 i) final;
        MAR_NO_DISCARD virtual uint32 getIndicesCount() const final;

    protected:

        int32 p_vboIndex{ -1 };
        int32 p_transformIndex{ -1 };
        int32 p_shaderIndex{ -1 };
        int32 p_iboIndex{ -1 };
        int32 p_camIndex{ -1 };
        int32 p_pointLightIndex{ -1 };

    };


    class FPipelineMeshColor : public FPipelineMesh {
    public:

        virtual void passColorSSBO(int32 i) final;

    protected:

        int32 p_colorIndex{ -1 };

    };


    class FPipelineMeshTex2D : public FPipelineMesh {
    public:

        virtual void passTexture(int32 i) final;

    protected:

        std::array<int32, 32> m_textures;
        uint32 m_texturesIndex{ 0 };

    };


    class FPipelineStorage : public IPipelineStorage {

    };


    class FPipelineFactory : public IPipelineFactory {
    public:

        void fillPipelineFor(FPipelineMeshColor* const pPipeline,
                             FMeshBatchStaticColor* const pBatch) const final;

        void fillPipelineFor(FPipelineMeshTex2D* const pPipeline,
                             FMeshBatchStaticTex2D* const pBatch) const final;

        template<typename TMeshBatch>
        FPipelineMesh* emplaceMeshAndFill(TMeshBatch* pMeshBatch);

    };


}


#include "../private/Pipeline.inl"


#endif //MARENGINE_IGRAPHICSPIPELINE_H
