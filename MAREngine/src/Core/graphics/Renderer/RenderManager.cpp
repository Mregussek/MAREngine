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
    static void createPipelineForBatch(FGraphicsContextFactory* pFactory,
                                       FStoredPipelineInfo& pipelineInfo,
                                       TMeshBatch& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticColor>(FGraphicsContextFactory* pFactory,
                                                              FStoredPipelineInfo& pipelineInfo,
                                                              FMeshBatchStaticColor& batch);
    
    template<>
    static void createPipelineForBatch<FMeshBatchStaticTexture2D>(FGraphicsContextFactory* pFactory,
                                                                  FStoredPipelineInfo& pipelineInfo,
                                                                  FMeshBatchStaticTexture2D& batch);

    static uint32_t createCameraSSBO(FGraphicsContextFactory* pFactory, const RenderCamera* pRenderCamera);

    static uint32_t createPointLightSSBO(FGraphicsContextFactory* pFactory,
                                         const FPointLightBatch& pointLightBatch);



    void FRenderManager::create(FGraphicsContext* pGraphicsContext, Scene* pScene) {
        m_pGraphicsContext = pGraphicsContext;
        reset();
        pushSceneToRender(pScene);
        onBatchesReadyToDraw();
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


    }

    void FRenderManager::pushSceneToRender(Scene* pScene) {
        const FEntityArray& entities{ pScene->getEntities() };
        for(const Entity& entity : entities) {
            pushEntityToRender(entity);
        }
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
        FShaderBuffer* cameraSSBO{ m_pGraphicsContext->getFactory()->retrieveSSBO(m_cameraIndex) };
        cameraSSBO->update(maths::mat4::value_ptr(mvp), 0, sizeof(maths::mat4));
    }

    void FRenderManager::onBatchesReadyToDraw() {
        FGraphicsContextFactory* pFactory{ m_pGraphicsContext->getFactory() };

        m_cameraIndex = createCameraSSBO(pFactory, m_pRenderCamera);
        m_pointLightIndex = createPointLightSSBO(pFactory, m_pointLightBatch);

        for(FMeshBatchStaticColor& batch : m_meshesBatchColor) {
            FStoredPipelineInfo& pipelineInfo{ m_pipelines.emplace_back() };
            createPipelineForBatch(pFactory, pipelineInfo, batch);
            pipelineInfo.pPipeline->passCameraSSBO(pFactory->retrieveSSBO(m_cameraIndex));
        }
        //for(FMeshBatchStaticTexture2D& batch : m_meshesBatchTexture2D) {
        //    createPipelineForBatch(pFactory, batch);
        //}
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

    static void fillDefaultVertexLayout(FContextEmplacedInfo<FVertexBuffer>& vertexInfo) {
        FVertexInputLayoutInfo& positionInfo{ vertexInfo.pType->emplaceInputLayoutInfoElement() };
        positionInfo.inputType = EBufferInputType::VEC3;
        positionInfo.location = 0;
        positionInfo.offset = offsetof(Vertex, Vertex::position);

        FVertexInputLayoutInfo& normalInfo{ vertexInfo.pType->emplaceInputLayoutInfoElement() };
        normalInfo.inputType = EBufferInputType::VEC3;
        normalInfo.location = 1;
        normalInfo.offset = offsetof(Vertex, Vertex::lightNormal);

        FVertexInputLayoutInfo& texCoordsInfo{ vertexInfo.pType->emplaceInputLayoutInfoElement() };
        texCoordsInfo.inputType = EBufferInputType::VEC2;
        texCoordsInfo.location = 2;
        texCoordsInfo.offset = offsetof(Vertex, Vertex::textureCoordinates);

        FVertexInputLayoutInfo& shapeIndexInfo{ vertexInfo.pType->emplaceInputLayoutInfoElement() };
        shapeIndexInfo.inputType = EBufferInputType::FLOAT;
        shapeIndexInfo.location = 3;
        shapeIndexInfo.offset = offsetof(Vertex, Vertex::shapeID);

        FVertexInputDescription& vertexLayoutInfo{ vertexInfo.pType->getInputDescription() };
        vertexLayoutInfo.binding = 0;
        vertexLayoutInfo.stride = sizeof(Vertex);
    }

    static void fillDefaultTransformSSBO(FContextEmplacedInfo<FShaderBuffer>& transformInfo,
                                         uint32_t bindingPoint) {
        FShaderInputLayoutInfo& layoutInfo{ transformInfo.pType->getInputLayoutInfo() };
        layoutInfo.binding = bindingPoint;
        layoutInfo.shaderStage = EShaderStage::VERTEX;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem& item{ transformInfo.pType->emplaceItem() };
        item.count = 32;
        item.inputType = EBufferInputType::MAT4;
        item.name = "Transforms.Transform[32]";
        item.offset = 0;
        item.memoryUsed = 32 * sizeof(maths::mat4);
    }

    static void fillDefaultColorSSBO(FContextEmplacedInfo<FShaderBuffer>& colorInfo) {
        FShaderInputLayoutInfo& layoutInfo{ colorInfo.pType->getInputLayoutInfo() };
        layoutInfo.binding = 3;
        layoutInfo.shaderStage = EShaderStage::FRAGMENT;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem& item{ colorInfo.pType->emplaceItem() };
        item.count = 32;
        item.inputType = EBufferInputType::VEC4;
        item.name = "Colors.Color[32]";
        item.offset = 0;
        item.memoryUsed = 32 * sizeof(maths::vec4);
    }

    template<typename TMeshBatch>
    void createPipelineForBatch(FGraphicsContextFactory* pFactory,
                                FStoredPipelineInfo& pipelineInfo, TMeshBatch& batch) { }

    template<>
    void createPipelineForBatch<FMeshBatchStaticColor>(FGraphicsContextFactory* pFactory,
                                                       FStoredPipelineInfo& pipelineInfo,
                                                       FMeshBatchStaticColor& batch) {
        FContextEmplacedInfo<FGraphicsPipelineColorMesh> meshPipelineInfo =
            pFactory->emplaceGraphicsPipelineColorMesh();
        FContextEmplacedInfo<FVertexBuffer> vertexInfo =
            pFactory->emplaceVertexBuffer();
        FContextEmplacedInfo<FIndexBuffer> indexInfo =
            pFactory->emplaceIndexBuffer();
        FContextEmplacedInfo<FShaderBuffer> transformInfo =
            pFactory->emplaceShaderStorageBuffer();
        FContextEmplacedInfo<FShaderBuffer> colorInfo =
            pFactory->emplaceShaderStorageBuffer();
        FContextEmplacedInfo<FShaderPipeline> shaderInfo =
            pFactory->emplaceShaderPipeline();

        fillDefaultVertexLayout(vertexInfo);
        vertexInfo.pType->create(GraphicLimits::sizeOfVertices);
        vertexInfo.pType->update(batch.getVertices());
        batch.setUniquePipelineID(vertexInfo.index);

        indexInfo.pType->create(GraphicLimits::sizeOfIndices);
        indexInfo.pType->update(batch.getIndices());

        const FTransformsArray& transforms{ batch.getTransforms() };
        fillDefaultTransformSSBO(transformInfo, 5);
        transformInfo.pType->create();
        transformInfo.pType->update(
            maths::mat4::value_ptr(transforms), 0, transforms.size() * sizeof(maths::mat4)
        );
        batch.seUniqueTransformsID(transformInfo.index);

        const FColorsArray& colors{ batch.getColors() };
        fillDefaultColorSSBO(colorInfo);
        colorInfo.pType->create();
        colorInfo.pType->update(
            maths::vec4::value_ptr(colors), 0, colors.size() * sizeof(maths::vec4)
        );
        batch.setUniqueColorsID(colorInfo.index);

        shaderInfo.pType->passVertexShader("resources/shaders/color.vert.glsl");
        shaderInfo.pType->passFragmentShader("resources/shaders/color.frag.glsl");
        shaderInfo.pType->compile();

        meshPipelineInfo.pType->passVertexBuffer(vertexInfo.pType);
        meshPipelineInfo.pType->passIndexBuffer(indexInfo.pType);
        meshPipelineInfo.pType->passTransformSSBO(transformInfo.pType);
        meshPipelineInfo.pType->passColorSSBO(colorInfo.pType);
        meshPipelineInfo.pType->passShaderPipeline(shaderInfo.pType);

        pipelineInfo.pPipeline = meshPipelineInfo.pType;
        pipelineInfo.type = EGraphicsPipelineType::MESH_COLOR;
        pipelineInfo.index = meshPipelineInfo.index;
    }

    template<>
    void createPipelineForBatch<FMeshBatchStaticTexture2D>(FGraphicsContextFactory* pFactory,
                                                           FStoredPipelineInfo& pipelineInfo,
                                                           FMeshBatchStaticTexture2D& batch) {

    }

    static uint32_t createCameraSSBO(FGraphicsContextFactory* pFactory,
                                     const RenderCamera* pRenderCamera) {
        FContextEmplacedInfo<FShaderBuffer> cameraInfo{ pFactory->emplaceShaderStorageBuffer() };

        FShaderInputLayoutInfo& layoutInfo{ cameraInfo.pType->getInputLayoutInfo() };
        layoutInfo.binding = 0;
        layoutInfo.shaderStage = EShaderStage::VERTEX;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem& cameraMvpItem{ cameraInfo.pType->emplaceItem() };
        cameraMvpItem.count = 1;
        cameraMvpItem.inputType = EBufferInputType::MAT4;
        cameraMvpItem.name = "Camera.MVP";
        cameraMvpItem.offset = 0;
        cameraMvpItem.memoryUsed = sizeof(maths::mat4);

        const maths::mat4& mvp{ pRenderCamera->getMVP() };
        cameraInfo.pType->create();
        cameraInfo.pType->update(maths::mat4::value_ptr(mvp), cameraMvpItem.offset, sizeof(maths::mat4));

        return cameraInfo.index;
    }

    uint32_t createPointLightSSBO(FGraphicsContextFactory* pFactory,
                                  const FPointLightBatch& pointLightBatch) {
        FContextEmplacedInfo<FShaderBuffer> lightInfo{ pFactory->emplaceShaderStorageBuffer() };

        FShaderInputLayoutInfo& layoutInfo{ lightInfo.pType->getInputLayoutInfo() };
        layoutInfo.binding = 2;
        layoutInfo.shaderStage = EShaderStage::FRAGMENT;
        layoutInfo.bufferType = EBufferType::SSBO;

        FShaderBufferItem& pointLightMaterialItem{ lightInfo.pType->emplaceItem() };
        pointLightMaterialItem.count = 32;
        pointLightMaterialItem.inputType = EBufferInputType::OTHER;
        pointLightMaterialItem.name = "PointLigts.LightMaterial[32]";
        pointLightMaterialItem.offset = 0;
        pointLightMaterialItem.memoryUsed = 32 * sizeof(FPointLight);

        FShaderBufferItem& materialSizeItem{ lightInfo.pType->emplaceItem() };
        materialSizeItem.count = 1;
        materialSizeItem.inputType = EBufferInputType::INT;
        materialSizeItem.name = "PointLigts.LightMaterialSize";
        materialSizeItem.offset = pointLightMaterialItem.memoryUsed;
        materialSizeItem.memoryUsed = 1 * sizeof(int32_t);

        const auto& pointLights{ pointLightBatch.getLights() };
        const int32_t lightSize{ (int32_t)pointLights.size() };
        lightInfo.pType->create();
        lightInfo.pType->update(&pointLights.at(0).position.x,
                                pointLightMaterialItem.offset,
                                lightSize * sizeof(FPointLight));
        lightInfo.pType->update(&lightSize,
                                materialSizeItem.offset,
                                sizeof(int32_t));

        return lightInfo.index;
    }


}
