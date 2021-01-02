/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "RenderManager.h"
#include "RenderManagerEvents.h"
#include "RenderMemorizer.h"
#include "RenderCamera.h"
#include "../GraphicLimits.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Scene.h"
#include "../../ecs/Entity/Entity.h"


namespace marengine {


	FRenderManager* FRenderManager::Instance{ nullptr };


	void FRenderManager::reset() {
		m_staticColorMeshBatch.reset();
		m_staticTexture2DMeshBatch.reset();
		m_pointLightBatch.reset();
		m_renderCamera = nullptr;
	}

	void FRenderManager::batchEntities(const std::vector<Entity>& entities) {
		for (const Entity& entity : entities) {
			submitEntityRenderableToBatch(entity);
			submitEntityLightToBatch(entity);
			submitEntityCamera(entity);
		}

		onBatchesReadyToDraw();
	}

	FRenderManager::SubmitRenderableReturnInfo FRenderManager::submitEntityRenderableToBatch(const Entity& entity) {
		if (m_staticColorMeshBatch.canBeBatched(entity)) {
			m_staticColorMeshBatch.submitToBatch(entity);
			return { true, &m_staticColorMeshBatch };
		}

		if (m_staticTexture2DMeshBatch.canBeBatched(entity)) {
			m_staticTexture2DMeshBatch.submitToBatch(entity);
			return { true, &m_staticTexture2DMeshBatch };
		}

		return { false, nullptr };
	}

	bool FRenderManager::submitEntityLightToBatch(const Entity& entity) {
		if (m_pointLightBatch.canBeBatched(entity)) {
			m_pointLightBatch.submitEntityWithLightning(entity);
			return true;
		}

		return false;
	}

	bool FRenderManager::submitEntityCamera(const Entity& entity) {
		if (containsMainCamera(entity)) {
			const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			m_renderCamera = &cameraComponent.renderCamera;
			return true;
		}

		return false;
	}

	bool FRenderManager::containsMainCamera(const Entity& entity) const {
		if (entity.hasComponent<CameraComponent>()) {
			const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			if (cameraComponent.isMainCamera()) {
				return true;
			}
		}
		
		return false;
	}

	void FRenderManager::onBatchesReadyToDraw() {
		createBuffers();
		fillBuffers();
	}

	void FRenderManager::createBuffers() {
		PipelineStorage::Instance->close();
		ShaderBufferStorage::Instance->close();

		{ // handle static color mesh batches
			const uint32_t uniquePipelineID{ createVertexIndexBuffers(&m_staticColorMeshBatch) };
			m_staticColorMeshBatch.setUniquePipelineID(uniquePipelineID);

			const uint32_t uniqueTransformsSSBO{ createTransformsSSBO() };
			m_staticColorMeshBatch.setTransformsSSBOindex(uniqueTransformsSSBO);

			const uint32_t uniqueColorsSSBO{ createColorSSBO() };
			m_staticColorMeshBatch.setColorsSSBOindex(uniqueColorsSSBO);
		}

		{ // handle texture 2d static mesh batches
			const uint32_t uniquePipelineID{ createVertexIndexBuffers(&m_staticTexture2DMeshBatch) };
			m_staticTexture2DMeshBatch.setUniquePipelineID(uniquePipelineID);

			const uint32_t uniqueTransformsSSBO{ createTransformsSSBO() };
			m_staticTexture2DMeshBatch.setTransformsSSBOindex(uniqueTransformsSSBO);
		}

		{ // handle point lights batches
			const uint32_t uniquePointLightSSBOindex{ createPointLightSSBO() };
			m_pointLightBatch.setPointLightSSBOindex(uniquePointLightSSBOindex);
		}

		{ // handle camera
			const uint32_t uniqueCameraSSBO{ createCameraSSBO() };
			RenderMemorizer::Instance->cameraSSBO = uniqueCameraSSBO;
		}
	}

	void FRenderManager::fillBuffers() {
		{ // fill static color mesh batch
			FRenderManagerEvents::onVertexIndexBuffersUpdate(m_staticColorMeshBatch);
			FRenderManagerEvents::onTransformsUpdate(m_staticColorMeshBatch);
			FRenderManagerEvents::onColorsUpdate(m_staticColorMeshBatch);
		}
		{ // fill texture2d static mesh batch
			FRenderManagerEvents::onVertexIndexBuffersUpdate(m_staticTexture2DMeshBatch);
			FRenderManagerEvents::onTransformsUpdate(m_staticTexture2DMeshBatch);
		}
		{ // fill point light batch
			FRenderManagerEvents::onPointLightUpdate(m_pointLightBatch);
		}
		{ // fill camera ssbo
			FRenderManagerEvents::onRenderCameraUpdate(m_renderCamera);
		}
	}

	uint32_t FRenderManager::createVertexIndexBuffers(FMeshBatchStatic* meshBatch) const {
		PipelineOpenGL& pipeline{ PipelineStorage::Instance->createPipeline() };
		pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);

		return PipelineStorage::Instance->getPipelines().size() - 1;
	}

	uint32_t FRenderManager::createTransformsSSBO() const {
		const std::vector<UniformItem> transformItems{ GLSL_SSBOs::ut_u_SeparateTransform };
		ShaderBufferStorageOpenGL& transformsSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		transformsSSBO.initialize(GLSL_SSBOs::ub_EntityCmp, transformItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createColorSSBO() const {
		const std::vector<UniformItem> colorsItems{ GLSL_SSBOs::ut_u_Color };
		ShaderBufferStorageOpenGL& colorsSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		colorsSSBO.initialize(GLSL_SSBOs::ub_TextureSamplers, colorsItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createPointLightSSBO() const {
		const std::vector<UniformItem> pointLightItems{ GLSL_SSBOs::ut_u_material, GLSL_SSBOs::ut_u_lightSize };
		ShaderBufferStorageOpenGL& pointLightSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		pointLightSSBO.initialize(GLSL_SSBOs::ub_Material, pointLightItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createCameraSSBO() const {
		const std::vector<UniformItem> cameraItems{ GLSL_SSBOs::ut_u_MVP };
		ShaderBufferStorageOpenGL& cameraSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		cameraSSBO.initialize(GLSL_SSBOs::ub_Camera, cameraItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	const FMeshBatchStaticColor& FRenderManager::getStaticColorBatch() const {
		return m_staticColorMeshBatch;
	}

	const FMeshBatchStaticTexture2D& FRenderManager::getStaticTexture2DBatch() const {
		return m_staticTexture2DMeshBatch;
	}

	const FPointLightBatch& FRenderManager::getPointLightsBatch() const {
		return m_pointLightBatch;
	}

	void FRenderManager::setRenderCamera(const RenderCamera* renderCamera) {
		m_renderCamera = renderCamera;
		FRenderManagerEvents::onRenderCameraUpdate(renderCamera);
	}


}
