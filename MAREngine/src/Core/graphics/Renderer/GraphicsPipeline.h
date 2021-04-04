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


#include "IGraphicsContext.h"
#include <vector>


namespace marengine {

    class FVertexBuffer;
    class FIndexBuffer;
    class FShaderBuffer;
    class FShaderPipeline;

   
    class FGraphicsPipeline : public IGraphicsPipeline {
    public:

    };


    class FGraphicsPipelineMesh : public FGraphicsPipeline {
    public:

        virtual void passVertexBuffer(FVertexBuffer* pVertexBuffer) final;
        virtual void passIndexBuffer(FIndexBuffer* pIndexBuffer) final;
        virtual void passShaderBuffers(std::vector<FShaderBuffer>* pShaderBuffers) final;
        virtual void passShaderPipeline(FShaderPipeline* pShaderPipeline) final;

    protected:

        std::vector<FShaderBuffer>* p_pShaderBuffers{ nullptr };
        FShaderPipeline* p_pShaderPipeline{ nullptr };
        FVertexBuffer* p_pVertexBuffer{ nullptr };
        FIndexBuffer* p_pIndexBuffer{ nullptr };

    };


    class FGraphicsPipelineColorMesh : public FGraphicsPipelineMesh {

    };


    class FGraphicsPipelineTexture2DMesh : public FGraphicsPipelineMesh {

    };


}


#endif //MARENGINE_IGRAPHICSPIPELINE_H
