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


#include "RenderBufferManager.h"
#include "RenderCamera.h"
#include "../Mesh/MeshBatchStatic.h"
#include "../Lightning/PointLightBatch.h"
#include "../Renderer/PipelineManager.h"
#include "../GraphicsLimits.h"
#include "../Renderer/RenderMemorizer.h"


namespace marengine {


	void FRenderBufferManager::onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticColor>& colorBatches) {
		for (auto& batch : colorBatches) {
			createPipeline(batch);
			createTransformColorSSBO(batch);
			onCreateColorSSBO(batch);
		}
	}

	void FRenderBufferManager::onMeshBatchReadyToDraw(std::vector<FMeshBatchStaticTexture2D>& textureBatches) {
		for (auto& batch : textureBatches) {
			createPipeline(batch);
			createTransformTexture2DSSBO(batch);
		}
	}

	void FRenderBufferManager::onPipelineUpdate(const FMeshBatchStatic& staticBatch) {
		const auto& pipeline{ FPipelineManager::Instance->getPipeline(staticBatch.getUniquePipelineID()) };
		pipeline.bind();
		pipeline.update(staticBatch.getVertices(), staticBatch.getIndices());
	}

	void FRenderBufferManager::onTransformsUpdate(const FMeshBatchStatic& staticBatch) {
		const auto& transforms{ staticBatch.getTransforms() };

		const auto& transformSSBO{ FPipelineManager::Instance->getSSBO(staticBatch.getUniqueTransformsID()) };
		transformSSBO.bind();
		transformSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
	}

	void FRenderBufferManager::onCreateColorSSBO(FMeshBatchStaticColor& colorBatch) {
		const std::vector<UniformItem> colorItems{ GLSLShaderInfo::Colors };
		auto& colorSSBO = FPipelineManager::Instance->createSSBO();
		colorSSBO.initialize(GLSLShaderInfo::ColorsSSBO, colorItems);
		colorBatch.setUniqueColorsID(FPipelineManager::Instance->getSSBOs().size() - 1);

		onColorUpdate(colorBatch);
	}

	void FRenderBufferManager::onColorUpdate(const FMeshBatchStaticColor& colorBatch) {
		const auto& colors{ colorBatch.getColors() };

		const auto& colorSSBO{ FPipelineManager::Instance->getSSBO(colorBatch.getUniqueColorsID()) };
		colorSSBO.bind();
		colorSSBO.update<float>(GLSLShaderInfo::Colors.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
	}

	void FRenderBufferManager::onCreatePointLightsSSBO(FPointLightBatch& pointLightBatch) {
		const std::vector<UniformItem> pointLightsItems{ GLSLShaderInfo::LightMaterial, GLSLShaderInfo::LightMaterialSize };
		auto& pointLightsSSBO = FPipelineManager::Instance->createSSBO();
		pointLightsSSBO.initialize(GLSLShaderInfo::PointLightSSBO, pointLightsItems);
		pointLightBatch.setUniquePointLightID(FPipelineManager::Instance->getSSBOs().size() - 1);

		onPointLightUpdate(pointLightBatch);
	}

	void FRenderBufferManager::onPointLightUpdate(const FPointLightBatch& pointLightBatch) {
		const auto& pointLights{ pointLightBatch.getLights() };
		const int32_t lightSize{ (int32_t)pointLights.size() };

		const auto& pointLightsSSBO{ FPipelineManager::Instance->getSSBO(pointLightBatch.getUniquePointLightID()) };
		pointLightsSSBO.bind();
		pointLightsSSBO.update<float>(GLSLShaderInfo::LightMaterial.offset, sizeof(FPointLight) * pointLights.size(), &pointLights[0].position.x);
		pointLightsSSBO.update<int32_t>(GLSLShaderInfo::LightMaterialSize.offset, sizeof(int32_t), &lightSize);
	}

	void FRenderBufferManager::onCreateRenderCameraSSBO(const RenderCamera* renderCamera) {
		const std::vector<UniformItem> cameraItems{ GLSLShaderInfo::MVP };
		auto& cameraSSBO = FPipelineManager::Instance->createSSBO();
		RenderMemorizer::Instance->cameraSSBO = FPipelineManager::Instance->getSSBOs().size() - 1;
		cameraSSBO.initialize(GLSLShaderInfo::CameraSSBO, cameraItems);

		onRenderCameraUpdate(renderCamera);
	}

	void FRenderBufferManager::onRenderCameraUpdate(const RenderCamera* renderCamera) {
		auto& cameraSSBO{ FPipelineManager::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSLShaderInfo::MVP.offset, sizeof(maths::mat4), maths::mat4::value_ptr(renderCamera->getMVP()));
	}

	void FRenderBufferManager::createPipeline(FMeshBatchStatic& staticBatch) {
		const uint32_t verticesMemory{ staticBatch.getVertices().size() * sizeof(Vertex) };
		const uint32_t indicesMemory{ staticBatch.getIndices().size() * sizeof(uint32_t) };

		auto& pipeline = FPipelineManager::Instance->createPipeline();
		pipeline.initialize(verticesMemory, indicesMemory);
		staticBatch.setUniquePipelineID(FPipelineManager::Instance->getPipelines().size() - 1);

		onPipelineUpdate(staticBatch);
	}

	void FRenderBufferManager::createTransformColorSSBO(FMeshBatchStatic& staticBatch) {
		const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
		auto& transformSSBO = FPipelineManager::Instance->createSSBO();
		transformSSBO.initialize(GLSLShaderInfo::TransformColorSSBO, transformItems);
		staticBatch.seUniqueTransformsID(FPipelineManager::Instance->getSSBOs().size() - 1);

		onTransformsUpdate(staticBatch);
	}

	void FRenderBufferManager::createTransformTexture2DSSBO(FMeshBatchStatic& staticBatch) {
		const std::vector<UniformItem> transformItems{ GLSLShaderInfo::Transform };
		auto& transformSSBO = FPipelineManager::Instance->createSSBO();
		transformSSBO.initialize(GLSLShaderInfo::TransformTexture2DSSBO, transformItems);
		staticBatch.seUniqueTransformsID(FPipelineManager::Instance->getSSBOs().size() - 1);

		onTransformsUpdate(staticBatch);
	}


}
