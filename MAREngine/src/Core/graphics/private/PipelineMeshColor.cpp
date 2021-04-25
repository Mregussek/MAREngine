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


#include "../public/Pipeline.h"
#include "../public/Buffer.h"
#include "../public/MeshBatch.h"


namespace marengine {


    static void fillDefaultVertexLayout(FVertexBuffer* const pVertexBuffer) {
        FVertexInputVariableInfo positionInfo;
        positionInfo.inputType = EInputType::VEC3;
        positionInfo.location = 0;
        positionInfo.offset = offsetof(Vertex, Vertex::position);

        FVertexInputVariableInfo normalInfo;
        normalInfo.inputType = EInputType::VEC3;
        normalInfo.location = 1;
        normalInfo.offset = offsetof(Vertex, Vertex::lightNormal);

        FVertexInputVariableInfo texCoordsInfo;
        texCoordsInfo.inputType = EInputType::VEC2;
        texCoordsInfo.location = 2;
        texCoordsInfo.offset = offsetof(Vertex, Vertex::textureCoordinates);

        FVertexInputVariableInfo shapeIndexInfo;
        shapeIndexInfo.inputType = EInputType::FLOAT;
        shapeIndexInfo.location = 3;
        shapeIndexInfo.offset = offsetof(Vertex, Vertex::shapeID);

        FVertexInputDescription inputDescription;
        inputDescription.binding = 0;
        inputDescription.stride = sizeof(Vertex);

        pVertexBuffer->setInputDescription(inputDescription);
        pVertexBuffer->pushVariableInfo(positionInfo);
        pVertexBuffer->pushVariableInfo(normalInfo);
        pVertexBuffer->pushVariableInfo(texCoordsInfo);
        pVertexBuffer->pushVariableInfo(shapeIndexInfo);
    }

    static void createPipelineVBO(FRenderContext* pContext,
                                  FPipelineMeshColor* pPipeline,
                                  FMeshBatchStaticColor* pBatch) {
        FVertexBuffer* const vertexBuffer{ pContext->getBufferFactory()->emplaceVBO() };

        fillDefaultVertexLayout(vertexBuffer);

        vertexBuffer->create(GraphicLimits::sizeOfVertices);
        vertexBuffer->update(pBatch->getVertices());

        pBatch->passVBO(vertexBuffer->getIndex());
        pPipeline->passVertexBuffer(vertexBuffer->getIndex());
    }

    static void createPipelineIBO(FRenderContext* pContext,
                                  FPipelineMeshColor* pPipeline,
                                  FMeshBatchStaticColor* pBatch) {
        FIndexBuffer* const indexBuffer{ pContext->getBufferFactory()->emplaceIBO() };

        const FIndicesArray& indices{ pBatch->getIndices() };
        indexBuffer->create(GraphicLimits::sizeOfIndices);
        indexBuffer->update(indices);
        indexBuffer->passIndicesCount(indices.size());

        pBatch->passIBO(indexBuffer->getIndex());
        pPipeline->passIndexBuffer(indexBuffer->getIndex());
    }

    static void fillDefaultTransformSSBO(FShaderBuffer* pTransformBuffer, uint32_t bindingPoint) {
        FShaderInputDescription description;
        description.binding = bindingPoint;
        description.shaderStage = EShaderStage::VERTEX;
        description.bufferType = EBufferType::SSBO;

        FShaderInputVariableInfo inputInfo;
        inputInfo.count = 32;
        inputInfo.inputType = EInputType::MAT4;
        inputInfo.name = "Transforms.Transform[32]";
        inputInfo.offset = 0;
        inputInfo.memoryUsed = 32 * sizeof(maths::mat4);

        pTransformBuffer->setInputDescription(description);
        pTransformBuffer->pushVariableInfo(inputInfo);
    }

    static void createPipelineTransformsSSBO(FRenderContext* pContext,
                                            FPipelineMeshColor* pPipeline,
                                            FMeshBatchStaticColor* pBatch) {
        FShaderBuffer* const transformSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        fillDefaultTransformSSBO(transformSSBO, 5);
        const FTransformsArray& transforms{ pBatch->getTransforms() };
        transformSSBO->create();
        transformSSBO->update(
                maths::mat4::value_ptr(transforms),
                0,
                transforms.size() * sizeof(maths::mat4)
        );

        pBatch->passTransformSSBO(transformSSBO->getIndex());
        pPipeline->passTransformSSBO(transformSSBO->getIndex());
    }

    static void fillDefaultColorSSBO(FShaderBuffer* pColorBuffer, uint32_t bindingPoint) {
        FShaderInputDescription description;
        description.binding = bindingPoint;
        description.shaderStage = EShaderStage::FRAGMENT;
        description.bufferType = EBufferType::SSBO;

        FShaderInputVariableInfo inputInfo;
        inputInfo.count = 32;
        inputInfo.inputType = EInputType::VEC4;
        inputInfo.name = "Colors.Color[32]";
        inputInfo.offset = 0;
        inputInfo.memoryUsed = 32 * sizeof(maths::vec4);

        pColorBuffer->setInputDescription(description);
        pColorBuffer->pushVariableInfo(inputInfo);
    }

    static void createPipelineColorSSBO(FRenderContext* pContext,
                                        FPipelineMeshColor* pPipeline,
                                        FMeshBatchStaticColor* pBatch) {
        FShaderBuffer* const colorSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        fillDefaultColorSSBO(colorSSBO, 3);
        const FColorsArray& colors{ pBatch->getColors() };
        colorSSBO->create();
        colorSSBO->update(
                maths::vec4::value_ptr(colors),
                0,
                colors.size() * sizeof(maths::vec4)
        );

        pBatch->passColorSSBO(colorSSBO->getIndex());
        pPipeline->passColorSSBO(colorSSBO->getIndex());
    }

    static void createPipelineShaders(FRenderContext* pContext,
                                      FPipelineMeshColor* pPipeline) {
        FShaders* pShaders{ pContext->getShadersFactory()->emplace() };
        pShaders->passVertex("resources/shaders/color.vert.glsl");
        pShaders->passFragment("resources/shaders/color.frag.glsl");
        pShaders->compile();
        pPipeline->passShaderPipeline(pShaders->getIndex());
    }


    void FPipelineFactory::fillPipelineFor(FPipelineMeshColor* pPipeline,
                                           FMeshBatchStaticColor* pBatch) const {
        createPipelineVBO(p_pRenderContext, pPipeline, pBatch);
        createPipelineIBO(p_pRenderContext, pPipeline, pBatch);
        createPipelineTransformsSSBO(p_pRenderContext, pPipeline, pBatch);
        createPipelineColorSSBO(p_pRenderContext, pPipeline, pBatch);
        createPipelineShaders(p_pRenderContext, pPipeline);

        pPipeline->passBufferStorage(p_pRenderContext->getBufferStorage());
        pPipeline->passShadersStorage(p_pRenderContext->getShadersStorage());
        pPipeline->passMaterialStorage(p_pRenderContext->getMaterialStorage());
        pPipeline->create();
    }


}
