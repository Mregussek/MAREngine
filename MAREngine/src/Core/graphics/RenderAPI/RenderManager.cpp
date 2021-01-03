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
#include "../GraphicsLimits.h"
#include "../GraphicsLogs.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"
#include "../../ecs/SceneManager.h"
#include "../../ecs/Scene.h"
#include "../../ecs/Entity/Entity.h"


namespace marengine {


	FRenderManager* FRenderManager::Instance{ nullptr };


	void FRenderManager::reset() {
		PipelineStorage::Instance->close();
		ShaderBufferStorage::Instance->close();

		m_staticColorMeshBatch.reset();
		m_staticTexture2DMeshBatch.reset();
		m_pointLightBatch.reset();
		m_renderCamera = nullptr;
	}

	void FRenderManager::batchEntities(const std::vector<Entity>& entities) {
		GRAPHICS_TRACE("F_RENDER_MANAGER: batching {} entities...", entities.size());

		for (const Entity& entity : entities) {
			submitEntityRenderableToBatch(entity);
			submitEntityLightToBatch(entity);
			submitEntityCamera(entity);
		}

		GRAPHICS_INFO("F_RENDER_MANAGER: {} entities are batched!", entities.size());

		onBatchesReadyToDraw();
	}

	FRenderManager::SubmitRenderableReturnInfo FRenderManager::submitEntityRenderableToBatch(const Entity& entity) {
		GRAPHICS_TRACE("F_RENDER_MANAGER: trying to submit {} entity with its renderable...", entity.getComponent<TagComponent>().tag);

		if (m_staticColorMeshBatch.canBeBatched(entity)) {
			GRAPHICS_TRACE("F_RENDER_MANAGER: {} entity will be submitted to static color mesh batch...", entity.getComponent<TagComponent>().tag);

			m_staticColorMeshBatch.submitToBatch(entity);
			return { true, &m_staticColorMeshBatch };
		}

		if (m_staticTexture2DMeshBatch.canBeBatched(entity)) {
			GRAPHICS_TRACE("F_RENDER_MANAGER: {} entity will be submitted to static texture2D mesh batch...", entity.getComponent<TagComponent>().tag);

			m_staticTexture2DMeshBatch.submitToBatch(entity);
			return { true, &m_staticTexture2DMeshBatch };
		}

		GRAPHICS_TRACE("F_RENDER_MANAGER: {} entity will not be submitted as it cannot be batched...", entity.getComponent<TagComponent>().tag);
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
		GRAPHICS_TRACE("F_RENDER_MANAGER: batches are ready to draw, creating and filling buffers...");

		createBuffers();
		fillBuffers();
	}

	void FRenderManager::createBuffers() {
		{ // handle static color mesh batches
			GRAPHICS_TRACE("F_RENDER_MANAGER: creating pipelines and SSBOs for static color mesh batch...");

			const uint32_t uniquePipelineID{ createVertexIndexBuffers() };
			m_staticColorMeshBatch.setUniquePipelineID(uniquePipelineID);

			const uint32_t uniqueTransformsSSBO{ createTransformsSSBO() };
			m_staticColorMeshBatch.setTransformsSSBOindex(uniqueTransformsSSBO);

			const uint32_t uniqueColorsSSBO{ createColorSSBO() };
			m_staticColorMeshBatch.setColorsSSBOindex(uniqueColorsSSBO);

			GRAPHICS_INFO("F_RENDER_MANAGER: created buffers with uniqueID: VertexIndexBuffers - {}, TransformsSSBO - {}, ColorsSSBO - {} for static color mesh batch",
				uniquePipelineID, uniqueTransformsSSBO, uniqueColorsSSBO);
		}

		{ // handle texture 2d static mesh batches
			GRAPHICS_TRACE("F_RENDER_MANAGER: creating pipelines and SSBOs for static texture2d mesh batch...");

			const uint32_t uniquePipelineID{ createVertexIndexBuffers() };
			m_staticTexture2DMeshBatch.setUniquePipelineID(uniquePipelineID);

			const uint32_t uniqueTransformsSSBO{ createTransformsSSBO() };
			m_staticTexture2DMeshBatch.setTransformsSSBOindex(uniqueTransformsSSBO);

			GRAPHICS_INFO("F_RENDER_MANAGER: created buffers with uniqueID: VertexIndexBuffers - {}, TransformsSSBO - {} for static texture2D mesh batch",
				uniquePipelineID, uniqueTransformsSSBO);
		}

		{ // handle point lights batches
			GRAPHICS_TRACE("F_RENDER_MANAGER: creating SSBO for point lights batch...");

			const uint32_t uniquePointLightSSBOindex{ createPointLightSSBO() };
			m_pointLightBatch.setPointLightSSBOindex(uniquePointLightSSBOindex);

			GRAPHICS_INFO("F_RENDER_MANAGER: created buffers with uniqueID: PointLightsSSBO - {} for point lights batch", uniquePointLightSSBOindex);
		}

		{ // handle camera
			GRAPHICS_TRACE("F_RENDER_MANAGER: creating SSBO for render camera...");

			const uint32_t uniqueCameraSSBO{ createCameraSSBO() };
			RenderMemorizer::Instance->cameraSSBO = uniqueCameraSSBO;

			GRAPHICS_INFO("F_RENDER_MANAGER: created buffers with uniqueID: CameraSSBO - {} for Render Camera", uniqueCameraSSBO);
		}
	}

	void FRenderManager::fillBuffers() {
		if (m_staticColorMeshBatch.hasAnythingToDraw()) {
			FRenderManagerEvents::onVertexIndexBuffersUpdate(m_staticColorMeshBatch);
			FRenderManagerEvents::onTransformsUpdate(m_staticColorMeshBatch);
			FRenderManagerEvents::onColorsUpdate(m_staticColorMeshBatch);
		}
		if (m_staticTexture2DMeshBatch.hasAnythingToDraw()) {
			FRenderManagerEvents::onVertexIndexBuffersUpdate(m_staticTexture2DMeshBatch);
			FRenderManagerEvents::onTransformsUpdate(m_staticTexture2DMeshBatch);
			// TODO: should add texture loading
		}
		{ // fill point light batch
			FRenderManagerEvents::onPointLightUpdate(m_pointLightBatch);
		}
		{ // fill camera ssbo
			FRenderManagerEvents::onRenderCameraUpdate(m_renderCamera);
		}
	}

	uint32_t FRenderManager::createVertexIndexBuffers() const {
		PipelineOpenGL& pipeline{ PipelineStorage::Instance->createPipeline() };
		pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);

		return PipelineStorage::Instance->getPipelines().size() - 1;
	}

	uint32_t FRenderManager::createTransformsSSBO() const {
		const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
		ShaderBufferStorageOpenGL& transformsSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		transformsSSBO.initialize(GLSLShaderInfo::TransformSSBO, transformItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createColorSSBO() const {
		const std::vector<UniformItem> colorsItems{ GLSLShaderInfo::Colors };
		ShaderBufferStorageOpenGL& colorsSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		colorsSSBO.initialize(GLSLShaderInfo::ColorsSSBO, colorsItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createPointLightSSBO() const {
		const std::vector<UniformItem> pointLightItems{ GLSLShaderInfo::LightMaterial, GLSLShaderInfo::LightMaterialSize };
		ShaderBufferStorageOpenGL& pointLightSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		pointLightSSBO.initialize(GLSLShaderInfo::PointLightSSBO, pointLightItems);

		return ShaderBufferStorage::Instance->getSSBOs().size() - 1;
	}

	uint32_t FRenderManager::createCameraSSBO() const {
		const std::vector<UniformItem> cameraItems{ GLSLShaderInfo::MVP };
		ShaderBufferStorageOpenGL& cameraSSBO{ ShaderBufferStorage::Instance->createShaderBufferStorage() };
		cameraSSBO.initialize(GLSLShaderInfo::CameraSSBO, cameraItems);

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
