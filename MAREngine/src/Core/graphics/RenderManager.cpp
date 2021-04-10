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
#include "Shaders.h"
#include "Pipeline.h"
#include "RenderCamera.h"
#include "../ecs/Entity/Entity.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity/EventsCameraEntity.h"


namespace marengine {

    template<typename TMeshBatch>
    static uint32_t getAvailableBatch(std::vector<TMeshBatch>& batch, const Entity& entity);

    template<typename TMeshBatch>
    [[maybe_unused]] static void createPipelineForBatch(FRenderContext* pContext,
                                                        TMeshBatch& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticColor>(FRenderContext* pContext,
                                                              FMeshBatchStaticColor& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticTexture2D>(FRenderContext* pContext,
                                                                  FMeshBatchStaticTexture2D& batch);

    static uint32_t createCameraSSBO(FRenderContext* pContext, const RenderCamera* pRenderCamera);

    static uint32_t createPointLightSSBO(FRenderContext* pContext,
                                         const FPointLightBatch& pointLightBatch);



    void FRenderManager::create(FRenderContext* pContext) {
        m_pContext = pContext;
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

		m_pContext->getBufferStorage()->reset();
		m_pContext->getPipelineStorage()->reset();
		m_pContext->getShadersStorage()->reset();
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
        FShaderBuffer* const cameraSSBO{ m_pContext->getBufferStorage()->getSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
    }

    void FRenderManager::onBatchesReadyToDraw() {
        m_cameraIndex = createCameraSSBO(m_pContext, m_pRenderCamera);
        m_pointLightIndex = createPointLightSSBO(m_pContext, m_pointLightBatch);

        const int8 batchSize{ (int8)m_meshesBatchColor.size() };
        for (int8 i = 0; i < batchSize; i++) {
            createPipelineForBatch(m_pContext, m_meshesBatchColor.at(i));
            FPipelineMeshColor* pPipeline{ m_pContext->getPipelineStorage()->getColorMesh(i) };
            pPipeline->passCameraSSBO(m_cameraIndex);
            pPipeline->passPointLightSSBO(m_pointLightIndex);
        }
    }



    template<typename TMeshBatch>
    uint32_t getAvailableBatch(std::vector<TMeshBatch>& batches, const Entity& entity) {
        auto canBatchEntity = [&entity](TMeshBatch& batch)->bool {
			return batch.canBeBatched(entity);
		};
		const auto validBatchIt = std::find_if(batches.begin(), batches.end(), canBatchEntity);
		if (validBatchIt != batches.end()) {
			return std::distance(batches.begin(), validBatchIt);
		}

		// If cannot find valid batch with place for Entity, emplace new one
		// This will cause a new draw call!
		batches.emplace_back();
		return batches.size() - 1;
    }

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

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineVBO(FRenderContext* pContext,
                                  TPipeline* pPipeline, TMeshBatch& batch) {
        FVertexBuffer* const vertexBuffer{ pContext->getBufferFactory()->emplaceVBO() };

        fillDefaultVertexLayout(vertexBuffer);

        vertexBuffer->create(GraphicLimits::sizeOfVertices);
        vertexBuffer->update(batch.getVertices());

        batch.setUniquePipelineID(vertexBuffer->getIndex());
        pPipeline->passVertexBuffer(vertexBuffer->getIndex());
    }

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineIBO(FRenderContext* pContext,
                                  TPipeline* pPipeline, TMeshBatch& batch) {
        FIndexBuffer* const indexBuffer{ pContext->getBufferFactory()->emplaceIBO() };

        const FIndicesArray& indices{ batch.getIndices() };
        indexBuffer->create(GraphicLimits::sizeOfIndices);
        indexBuffer->update(indices);
        indexBuffer->passIndicesCount(indices.size());

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

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineTransforms(FRenderContext* pContext,
                                         TPipeline* pPipeline, TMeshBatch& batch) {
        FShaderBuffer* const transformSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        fillDefaultTransformSSBO(transformSSBO, 5);
        const FTransformsArray& transforms{ batch.getTransforms() };
        transformSSBO->create();
        transformSSBO->update(
            maths::mat4::value_ptr(transforms),
            0,
            transforms.size() * sizeof(maths::mat4)
        );

        batch.seUniqueTransformsID(transformSSBO->getIndex());
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

    template<typename TMeshBatch, typename TPipeline>
    static void createPipelineColor(FRenderContext* pContext, TPipeline* pPipeline,
                                    TMeshBatch& batch) {
        FShaderBuffer* const colorSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        fillDefaultColorSSBO(colorSSBO, 3);
        const FColorsArray& colors{ batch.getColors() };
        colorSSBO->create();
        colorSSBO->update(
            maths::vec4::value_ptr(colors),
            0,
            colors.size() * sizeof(maths::vec4)
        );

        batch.setUniqueColorsID(colorSSBO->getIndex());
        pPipeline->passColorSSBO(colorSSBO->getIndex());
    }

    template<typename TMeshBatch>
    [[maybe_unused]] void createPipelineForBatch(FRenderContext* pContext, TMeshBatch& batch) { }

    template<>
    void createPipelineForBatch<FMeshBatchStaticColor>(FRenderContext* pContext,
                                                       FMeshBatchStaticColor& batch) {
        FPipelineMeshColor* const pPipeline =
                pContext->getPipelineFactory()->emplaceColorMesh();
        
        createPipelineVBO(pContext, pPipeline, batch);
        createPipelineIBO(pContext, pPipeline, batch);
        createPipelineTransforms(pContext, pPipeline, batch);
        createPipelineColor(pContext, pPipeline, batch);
        
        FShaders* pShaders{ pContext->getShadersFactory()->emplace() };
        pShaders->passVertex("resources/shaders/color.vert.glsl");
        pShaders->passFragment("resources/shaders/color.frag.glsl");
        pShaders->compile();
        pPipeline->passShaderPipeline(pShaders->getIndex());

        pPipeline->passBufferStorage(pContext->getBufferStorage());
        pPipeline->passShadersStorage(pContext->getShadersStorage());
        pPipeline->create();
    }

    template<>
    void createPipelineForBatch<FMeshBatchStaticTexture2D>(FRenderContext* pContext,
                                                           FMeshBatchStaticTexture2D& batch) {

    }

    static uint32_t createCameraSSBO(FRenderContext* pContext,
                                     const RenderCamera* pRenderCamera) {
        FShaderBuffer* const cameraSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        FShaderInputDescription description;
        description.binding = 0;
        description.shaderStage = EShaderStage::VERTEX;
        description.bufferType = EBufferType::SSBO;

        FShaderInputVariableInfo mvpInfo;
        mvpInfo.count = 1;
        mvpInfo.inputType = EInputType::MAT4;
        mvpInfo.name = "Camera.MVP";
        mvpInfo.offset = 0;
        mvpInfo.memoryUsed = sizeof(maths::mat4);

        cameraSSBO->setInputDescription(description);
        cameraSSBO->pushVariableInfo(mvpInfo);

        const maths::mat4& mvp{ pRenderCamera->getMVP() };
        cameraSSBO->create();
        cameraSSBO->update(maths::mat4::value_ptr(mvp), mvpInfo.offset, sizeof(maths::mat4));

        return cameraSSBO->getIndex();
    }

    uint32_t createPointLightSSBO(FRenderContext* pContext,
                                  const FPointLightBatch& pointLightBatch) {
        FShaderBuffer* const lightSSBO{ pContext->getBufferFactory()->emplaceSSBO() };

        FShaderInputDescription description;
        description.binding = 2;
        description.shaderStage = EShaderStage::FRAGMENT;
        description.bufferType = EBufferType::SSBO;

        FShaderInputVariableInfo materialInfo;
        materialInfo.count = 32;
        materialInfo.inputType = EInputType::OTHER;
        materialInfo.name = "PointLigts.LightMaterial[32]";
        materialInfo.offset = 0;
        materialInfo.memoryUsed = 32 * sizeof(FPointLight);

        FShaderInputVariableInfo sizeInfo;
        sizeInfo.count = 1;
        sizeInfo.inputType = EInputType::INT;
        sizeInfo.name = "PointLigts.LightMaterialSize";
        sizeInfo.offset = materialInfo.memoryUsed;
        sizeInfo.memoryUsed = 1 * sizeof(int32_t);

        lightSSBO->setInputDescription(description);
        lightSSBO->pushVariableInfo(materialInfo);
        lightSSBO->pushVariableInfo(sizeInfo);

        const auto& pointLights{ pointLightBatch.getLights() };
        const int32_t lightSize{ (int32_t)pointLights.size() };
        lightSSBO->create();
        lightSSBO->update(&pointLights.at(0).position.x,
                          materialInfo.offset,
                          lightSize * sizeof(FPointLight));
        lightSSBO->update(&lightSize,
                          sizeInfo.offset,
                          sizeof(int32_t));

        return lightSSBO->getIndex();
    }


}
