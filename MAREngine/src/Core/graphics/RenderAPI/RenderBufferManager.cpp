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


#include "RenderBufferManager.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../GraphicsLimits.h"
#include "RenderCamera.h"
#include "../Renderer/RenderMemorizer.h"


namespace marengine {


	template<typename TBatch>
	void vertexIndexBuffersUpdated(const TBatch& batch) {
		const auto& pipeline{ PipelineStorage::Instance->getPipeline(batch.getUniquePipelineID()) };
		pipeline.bind();
		pipeline.update(batch.getVertices(), batch.getIndices());
	}

	template<typename TBatch>
	void transformsUpdated(const TBatch& batch) {
		const auto& transforms{ batch.getTransforms() };

		const auto& transformSSBO{ ShaderBufferStorage::Instance->getSSBO(batch.getUniqueTransformsID()) };
		transformSSBO.bind();
		transformSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
	}

	template<typename TBatch>
	void colorsUpdated(const TBatch& batch) {
		const auto& colors{ batch.getColors() };

		const auto& colorSSBO{ ShaderBufferStorage::Instance->getSSBO(batch.getUniqueColorsID()) };
		colorSSBO.bind();
		colorSSBO.update<float>(GLSLShaderInfo::Colors.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
	}

	template<typename TBatch>
	void pointLightsUpdated(const TBatch& batch) {
		const auto& pointLights{ batch.getLights() };
		const int32_t lightSize{ (int32_t)pointLights.size() };

		const auto& pointLightsSSBO{ ShaderBufferStorage::Instance->getSSBO(batch.getUniquePointLightID()) };
		pointLightsSSBO.bind();
		pointLightsSSBO.update<float>(GLSLShaderInfo::LightMaterial.offset, sizeof(FPointLight) * pointLights.size(), &pointLights[0].position.x);
		pointLightsSSBO.update<int32_t>(GLSLShaderInfo::LightMaterialSize.offset, sizeof(int32_t), &lightSize);
	}


	template<typename TBatch>
	void createVertexIndexBuffers(TBatch& batch) {
		auto& pipeline = PipelineStorage::Instance->createPipeline();
		pipeline.initialize(batch.getVertices().size() * sizeof(Vertex), batch.getIndices().size() * sizeof(uint32_t));
		batch.setUniquePipelineID(PipelineStorage::Instance->getPipelines().size() - 1);

		vertexIndexBuffersUpdated(batch);
	}

	template<typename TBatch>
	void createColorsSSBO(TBatch& batch) {
		const std::vector<UniformItem> colorItems{ GLSLShaderInfo::Colors };
		auto& colorSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
		colorSSBO.initialize(GLSLShaderInfo::ColorsSSBO, colorItems);
		batch.setUniqueColorsID(ShaderBufferStorage::Instance->getSSBOs().size() - 1);

		colorsUpdated(batch);
	}

	template<typename TBatch>
	void createPointLightsSSBO(TBatch& batch) {
		const std::vector<UniformItem> pointLightsItems{ GLSLShaderInfo::LightMaterial, GLSLShaderInfo::LightMaterialSize };
		auto& pointLightsSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
		pointLightsSSBO.initialize(GLSLShaderInfo::PointLightSSBO, pointLightsItems);
		batch.setUniquePointLightID(ShaderBufferStorage::Instance->getSSBOs().size() - 1);

		pointLightsUpdated(batch);
	}


	void FRenderBufferManager::onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticColor>& colorBatches) {
		for (auto& batch : colorBatches) {
			createVertexIndexBuffers(batch);
			{
				const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
				auto& transformSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
				transformSSBO.initialize(GLSLShaderInfo::TransformColorSSBO, transformItems);
				batch.seUniqueTransformsID(ShaderBufferStorage::Instance->getSSBOs().size() - 1);

				transformsUpdated(batch); 
			}
			createColorsSSBO(batch);
		}
	}

	void FRenderBufferManager::onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticTexture2D>& textureBatches) {
		for (auto& batch : textureBatches) {
			createVertexIndexBuffers(batch);
			{
				const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
				auto& transformSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
				transformSSBO.initialize(GLSLShaderInfo::TransformTexture2DSSBO, transformItems);
				batch.seUniqueTransformsID(ShaderBufferStorage::Instance->getSSBOs().size() - 1);

				transformsUpdated(batch);
			}
		}
	}

	void FRenderBufferManager::onLightBatchReadyToDraw(std::vector<FPointLightBatch>& lightBatches) {
		for (auto& batch : lightBatches) {
			createPointLightsSSBO(batch);
		}
	}

	void FRenderBufferManager::onRenderCameraReadyToDraw(const RenderCamera* renderCamera) {
		const std::vector<UniformItem> cameraItems{ GLSLShaderInfo::MVP };
		auto& cameraSSBO = ShaderBufferStorage::Instance->createShaderBufferStorage();
		RenderMemorizer::Instance->cameraSSBO = ShaderBufferStorage::Instance->getSSBOs().size() - 1;
		cameraSSBO.initialize(GLSLShaderInfo::CameraSSBO, cameraItems);
	}

	void FRenderBufferManager::onPipelineUpdate(const FMeshBatchStaticColor& batch) {
		vertexIndexBuffersUpdated(batch);
	}

	void FRenderBufferManager::onTransformsUpdate(const FMeshBatchStaticColor& batch) {
		transformsUpdated(batch);
	}

	void FRenderBufferManager::onColorUpdate(const FMeshBatchStaticColor& batch) {
		colorsUpdated(batch);
	}

	void FRenderBufferManager::onPipelineUpdate(const FMeshBatchStaticTexture2D& batch) {
		vertexIndexBuffersUpdated(batch);
	}

	void FRenderBufferManager::onTransformsUpdate(const FMeshBatchStaticTexture2D& batch) {
		transformsUpdated(batch);
	}

	void FRenderBufferManager::onLightUpdate(const FPointLightBatch& light) {
		pointLightsUpdated(light);
	}

	void FRenderBufferManager::onRenderCameraUpdate(const RenderCamera* renderCamera) {
		auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSLShaderInfo::MVP.offset, sizeof(maths::mat4), maths::mat4::value_ptr(renderCamera->getMVP()));
	}


}
