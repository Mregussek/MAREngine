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


#ifndef MARENGINE_GRAPHICSPIPELINE_H
#define MARENGINE_GRAPHICSPIPELINE_H


#include "IGraphics.h"
#include <vector>


namespace marengine {

    class FVertexBuffer;
    class FIndexBuffer;
    class FShaderBuffer;
    class FShaderPipeline;


    class FGraphicsPipeline : public IGraphicsPipeline {
    public:

        virtual void passFactory(FGraphicsFactory* pFactory) final;

    protected:

        FGraphicsFactory* p_pFactory{ nullptr };

    };


    class FGraphicsPipelineMesh : public FGraphicsPipeline { 
    public:

        virtual void passIndexBuffer(int32_t i) final;
        virtual void passCameraSSBO(int32_t i) final;
        virtual void passPointLightSSBO(int32_t i) final;
        virtual uint32_t getIndicesCount() const final;

    protected:

        int32_t p_indexBufferIndex{ -1 };
        int32_t p_cameraBufferIndex{ -1 };
        int32_t p_pointLightBufferIndex{ -1 };

    };


    class FGraphicsPipelineColorMesh : public FGraphicsPipelineMesh {
    public:

        virtual void passVertexBuffer(int32_t i) final;
        virtual void passTransformSSBO(int32_t i) final;
        virtual void passColorSSBO(int32_t i) final;
        virtual void passShaderPipeline(int32_t i) final;

    protected:

        int32_t p_vertexBufferIndex{ -1 };
        int32_t p_transformBufferIndex{ -1 };
        int32_t p_colorBufferIndex{ -1 };
        int32_t p_shaderPipelineIndex{ -1 };

    };


    class FGraphicsPipelineTexture2DMesh : public FGraphicsPipelineMesh {

    };


}


#endif //MARENGINE_IGRAPHICSPIPELINE_H
