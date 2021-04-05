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


#include "RenderManager.h"
#include "Buffer.h"
#include "ShaderPipeline.h"
#include "GraphicsPipeline.h"
#include "../RenderAPI/RenderCamera.h"
#include "../GraphicsLimits.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Scene.h"
#include "../../ecs/Entity/EventsCameraEntity.h"


namespace marengine {

    template<typename TMeshBatch>
    static uint32_t getAvailableBatch(std::vector<TMeshBatch>& batch, const Entity& entity);

    template<typename TMeshBatch>
    static void createPipelineForBatch(FGraphicsFactory* pFactory,
                                       FGraphicsPipelineAtManagerInfo& pipelineInfo,
                                       TMeshBatch& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticColor>(FGraphicsFactory* pFactory,
                                                              FGraphicsPipelineAtManagerInfo& pipelineInfo,
                                                              FMeshBatchStaticColor& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticTexture2D>(FGraphicsFactory* pFactory,
                                                                  FGraphicsPipelineAtManagerInfo& pipelineInfo,
                                                                  FMeshBatchStaticTexture2D& batch);

    static uint32_t createCameraSSBO(FGraphicsFactory* pFactory, const RenderCamera* pRenderCamera);

    static uint32_t createPointLightSSBO(FGraphicsFactory* pFactory,
                                         const FPointLightBatch& pointLightBatch);



    void FRenderManager::create(FGraphicsContext* pGraphicsContext) {
        m_pGraphicsContext = pGraphicsContext;
    }

    template<typename TMeshBatch>
    static void clearMeshBatch(TMeshBatch& meshesBatch) {
        for (auto& batch : meshesBatch) { batch.reset(); }
        meshesBatch.clear();
    }

    void FRenderManager::reset() {
        clearMeshBatch(m_meshesBatchColor);
        clearMeshBatch(m_meshesBatchTexture2D);

		m_pointLightBatch.reset();

        FGraphicsFactory* pFactory{ m_pGraphicsContext->getFactory() };
        pFactory->reset();

        m_pipelines.clear();
    }

    void FRenderManager::pushSceneToRender(Scene* pScene) {
        reset();
        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            pushEntityToRender(entity);
        }
        onBatchesReadyToDraw();
    }

    void FRenderManager::pushEntityToRender(const Entity& entity) {
        if (entity.hasComponent<ColorComponent>()) {
			const uint32_t batchIndex{ getAvailableBatch(m_meshesBatchColor, entity) };
			auto& availableBatch{ m_meshesBatchColor.at(batchIndex) };
			availableBatch.submitToBatch(entity);

			auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
			meshBatchInfoComponent.batchIndex = batchIndex;
			meshBatchInfoComponent.batchType = availableBatch.getBatchType();
		}
		if (entity.hasComponent<Texture2DComponent>()) {
			const uint32_t batchIndex{ getAvailableBatch(m_meshesBatchTexture2D, entity) };
			auto& availableBatch{ m_meshesBatchTexture2D.at(batchIndex) };
			availableBatch.submitToBatch(entity);

			auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
			meshBatchInfoComponent.batchIndex = batchIndex;
			meshBatchInfoComponent.batchType = availableBatch.getBatchType();
		}
		if (entity.hasComponent<PointLightComponent>()) {
			if (m_pointLightBatch.canBeBatched(entity)) {
				m_pointLightBatch.submitEntityWithLightning(entity);

				auto& lightBatchInfoComponent{ entity.getComponent<LightBatchInfoComponent>() };
				lightBatchInfoComponent.batchType = m_pointLightBatch.getBatchType();
			}
		}
		if (entity.hasComponent<CameraComponent>()) {
		    auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			if (m_pRenderCamera && cameraComponent.isMainCamera()) {
			    FEventsCameraEntity::onMainCameraUpdate(entity);
			}
			else if(cameraComponent.isMainCamera()){
			    const auto& transformComponent{ entity.getComponent<TransformComponent>() };
			    RenderCamera* renderCamera{ &cameraComponent.renderCamera };
			    renderCamera->calculateCameraTransforms(transformComponent, cameraComponent);
			    m_pRenderCamera = renderCamera;
			}
		}
    }

    void FRenderManager::pushCameraToRender(const RenderCamera* pRenderCamera) {
        m_pRenderCamera = pRenderCamera;
        const maths::mat4& mvp{ m_pRenderCamera->getMVP() };
        FShaderBuffer* cameraSSBO{ m_pGraphicsContext->getFactory()->getSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
    }

    void FRenderManager::onBatchesReadyToDraw() {
        FGraphicsFactory* pFactory{ m_pGraphicsContext->getFactory() };

        m_cameraIndex = createCameraSSBO(pFactory, m_pRenderCamera);
        m_pointLightIndex = createPointLightSSBO(pFactory, m_pointLightBatch);

        for(FMeshBatchStaticColor& batch : m_meshesBatchColor) {
            FGraphicsPipelineAtManagerInfo& pipelineInfo{ m_pipelines.emplace_back() };
            createPipelineForBatch(pFactory, pipelineInfo, batch);
            pFactory->getPipelineColorMesh(pipelineInfo.index)->passCameraSSBO(m_cameraIndex);
            pFactory->getPipelineColorMesh(pipelineInfo.index)->passPointLightSSBO(m_pointLightIndex);
        }
        //for(FMeshBatchStaticTexture2D& batch : m_meshesBatchTexture2D) {
        //    createPipelineForBatch(pFactory, batch);
        //}
    }

    const FGraphicsPipelinesAtManagerArray& FRenderManager::getPipelines() const {
        return m_pipelines;
    }



    template<typename TMeshBatch>
    uint32_t getAvailableBatch(std::vector<TMeshBatch>& batches, const Entity& entity) {
        auto canBatchEntity = [&entity](TMeshBatch& batch) {
			return batch.canBeBatched(entity);
		};
		const auto it = std::find_if(batches.begin(), batches.end(), canBatchEntity);
		if (it != batches.end()) {
			return std::distance(batches.begin(), it);
		}

		batches.emplace_back();
		return batches.size() - 1;
    }

    static void fillDefaultVertexLayout(FVertexBuffer* pVertexBuffer) {
        FVertexInputLayoutInfo positionInfo;
        positionInfo.inputType = EBufferInputType::VEC3;
        positionInfo.location = 0;
        positionInfo.offset = offsetof(Vertex, Vertex::position);

        FVertexInputLayoutInfo normalInfo;
        normalInfo.inputType = EBufferInputType::VEC3;
        normalInfo.location = 1;
        normalInfo.offset = offsetof(Vertex, Vertex::lightNormal);

        FVertexInputLayoutInfo texCoordsInfo;
        texCoordsInfo.inputType = EBufferInputType::VEC2;
        texCoordsInfo.location = 2;
        texCoordsInfo.offset = offsetof(Vertex, Vertex::textureCoordinates);

        FVertexInputLayoutInfo shapeIndexInfo;
        shapeIndexInfo.inputType = EBufferInputType::FLOAT;
        shapeIndexInfo.location = 3;
        shapeIndexInfo.offset = offsetof(Vertex, Vertex::shapeID);

        FVertexInputDescription inputDescription;
        inputDescription.binding = 0;
        inputDescription.stride = sizeof(Vertex);

        pVertexBuffer->setInputDescription(inputDescription);
        pVertexBuffer->pushInputElement(positionInfo);
        pVertexBuffer->pushInputElement(normalInfo);
        pVertexBuffer->pushInputElement(texCoordsInfo);
        pVertexBuffer->pushInputElement(shapeIndexInfo);
    }

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineVBO(FGraphicsFactory* pFactory, TPipeline* pPipeline, TMeshBatch& batch) {
        FVertexBuffer* vertexBuffer{ pFactory->emplaceVBO() };
        fillDefaultVertexLayout(vertexBuffer);
        vertexBuffer->create(GraphicLimits::sizeOfVertices);
        vertexBuffer->update(batch.getVertices());
        const uint32_t vertexID{ pFactory->getCountVBO() - 1 };
        batch.setUniquePipelineID(vertexID);
        pPipeline->passVertexBuffer(vertexID);
    }

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineIBO(FGraphicsFactory* pFactory, TPipeline* pPipeline, TMeshBatch& batch) {
        FIndexBuffer* indexBuffer{ pFactory->emplaceIBO() };
        const FIndicesArray& indices{ batch.getIndices() };
        indexBuffer->create(GraphicLimits::sizeOfIndices);
        indexBuffer->update(indices);
        indexBuffer->passIndicesCount(indices.size());
        pPipeline->passIndexBuffer(pFactory->getCountIBO() - 1);
    }

    static void fillDefaultTransformSSBO(FShaderBuffer* pTransformBuffer, uint32_t bindingPoint) {
        FShaderInputLayoutInfo layoutInfo;
        layoutInfo.binding = bindingPoint;
        layoutInfo.shaderStage = EShaderStage::VERTEX;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem item;
        item.count = 32;
        item.inputType = EBufferInputType::MAT4;
        item.name = "Transforms.Transform[32]";
        item.offset = 0;
        item.memoryUsed = 32 * sizeof(maths::mat4);

        pTransformBuffer->setInputLayoutInfo(layoutInfo);
        pTransformBuffer->pushItem(item);
    }

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineTransforms(FGraphicsFactory* pFactory, TPipeline* pPipeline, TMeshBatch& batch) {
        FShaderBuffer* transformSSBO{ pFactory->emplaceSSBO() };
        fillDefaultTransformSSBO(transformSSBO, 5);
        const FTransformsArray& transforms{ batch.getTransforms() };
        transformSSBO->create();
        transformSSBO->update(
            maths::mat4::value_ptr(transforms), 0, transforms.size() * sizeof(maths::mat4)
        );
        const uint32_t transformID{ pFactory->getCountSSBO() - 1 };
        batch.seUniqueTransformsID(transformID);
        pPipeline->passTransformSSBO(transformID);
    }

    static void fillDefaultColorSSBO(FShaderBuffer* pColorBuffer, uint32_t bindingPoint) {
        FShaderInputLayoutInfo layoutInfo;
        layoutInfo.binding = bindingPoint;
        layoutInfo.shaderStage = EShaderStage::FRAGMENT;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem item;
        item.count = 32;
        item.inputType = EBufferInputType::VEC4;
        item.name = "Colors.Color[32]";
        item.offset = 0;
        item.memoryUsed = 32 * sizeof(maths::vec4);

        pColorBuffer->setInputLayoutInfo(layoutInfo);
        pColorBuffer->pushItem(item);
    }

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineColor(FGraphicsFactory* pFactory, TPipeline* pPipeline, TMeshBatch& batch) {
        FShaderBuffer* colorSSBO{ pFactory->emplaceSSBO() };
        fillDefaultColorSSBO(colorSSBO, 3);
        const FColorsArray& colors{ batch.getColors() };
        colorSSBO->create();
        colorSSBO->update(
            maths::vec4::value_ptr(colors), 0, colors.size() * sizeof(maths::vec4)
        );
        const uint32_t colorID{ pFactory->getCountSSBO() - 1 };
        batch.setUniqueColorsID(colorID);
        pPipeline->passColorSSBO(colorID);
    }

    template<typename TMeshBatch>
    void createPipelineForBatch(FGraphicsFactory* pFactory,
                                FGraphicsPipelineAtManagerInfo& pipelineInfo, TMeshBatch& batch) { }

    template<>
    void createPipelineForBatch<FMeshBatchStaticColor>(FGraphicsFactory* pFactory,
                                                       FGraphicsPipelineAtManagerInfo& pipelineInfo,
                                                       FMeshBatchStaticColor& batch) {
        FGraphicsPipelineColorMesh* graphicsPipeline{ pFactory->emplacePipelineColorMesh() };
        
        createPipelineVBO(pFactory, graphicsPipeline, batch);
        createPipelineIBO(pFactory, graphicsPipeline, batch);
        createPipelineTransforms(pFactory, graphicsPipeline, batch);
        createPipelineColor(pFactory, graphicsPipeline, batch);
        
        FShaderPipeline* shaderPipeline{ pFactory->emplaceShaderPipeline() };
        shaderPipeline->passVertexShader("resources/shaders/color.vert.glsl");
        shaderPipeline->passFragmentShader("resources/shaders/color.frag.glsl");
        shaderPipeline->compile();
        graphicsPipeline->passShaderPipeline(pFactory->getCountShaderPipeline() - 1);

        graphicsPipeline->passFactory(pFactory);
        graphicsPipeline->create();

        pipelineInfo.type = EGraphicsPipelineType::MESH_COLOR;
        pipelineInfo.index = pFactory->getCountPipelineColorMesh() - 1;
    }

    template<>
    void createPipelineForBatch<FMeshBatchStaticTexture2D>(FGraphicsFactory* pFactory,
                                                           FGraphicsPipelineAtManagerInfo& pipelineInfo,
                                                           FMeshBatchStaticTexture2D& batch) {

    }

    static uint32_t createCameraSSBO(FGraphicsFactory* pFactory,
                                     const RenderCamera* pRenderCamera) {
        FShaderBuffer* cameraSSBO{ pFactory->emplaceSSBO() };

        FShaderInputLayoutInfo layoutInfo;
        layoutInfo.binding = 0;
        layoutInfo.shaderStage = EShaderStage::VERTEX;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem cameraMvpItem;
        cameraMvpItem.count = 1;
        cameraMvpItem.inputType = EBufferInputType::MAT4;
        cameraMvpItem.name = "Camera.MVP";
        cameraMvpItem.offset = 0;
        cameraMvpItem.memoryUsed = sizeof(maths::mat4);

        cameraSSBO->setInputLayoutInfo(layoutInfo);
        cameraSSBO->pushItem(cameraMvpItem);

        const maths::mat4& mvp{ pRenderCamera->getMVP() };
        cameraSSBO->create();
        cameraSSBO->update(maths::mat4::value_ptr(mvp), cameraMvpItem.offset, sizeof(maths::mat4));

        return pFactory->getCountSSBO() - 1;
    }

    uint32_t createPointLightSSBO(FGraphicsFactory* pFactory,
                                  const FPointLightBatch& pointLightBatch) {
        FShaderBuffer* lightSSBO{ pFactory->emplaceSSBO() };

        FShaderInputLayoutInfo layoutInfo;
        layoutInfo.binding = 2;
        layoutInfo.shaderStage = EShaderStage::FRAGMENT;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem materialItem;
        materialItem.count = 32;
        materialItem.inputType = EBufferInputType::OTHER;
        materialItem.name = "PointLigts.LightMaterial[32]";
        materialItem.offset = 0;
        materialItem.memoryUsed = 32 * sizeof(FPointLight);

        FShaderBufferItem materialSizeItem;
        materialSizeItem.count = 1;
        materialSizeItem.inputType = EBufferInputType::INT;
        materialSizeItem.name = "PointLigts.LightMaterialSize";
        materialSizeItem.offset = materialItem.memoryUsed;
        materialSizeItem.memoryUsed = 1 * sizeof(int32_t);

        lightSSBO->setInputLayoutInfo(layoutInfo);
        lightSSBO->pushItem(materialItem);
        lightSSBO->pushItem(materialSizeItem);

        const auto& pointLights{ pointLightBatch.getLights() };
        const int32_t lightSize{ (int32_t)pointLights.size() };
        lightSSBO->create();
        lightSSBO->update(&pointLights.at(0).position.x,
                                materialItem.offset,
                                lightSize * sizeof(FPointLight));
        lightSSBO->update(&lightSize,
                                materialSizeItem.offset,
                                sizeof(int32_t));

        return pFactory->getCountSSBO() - 1;
    }


}
