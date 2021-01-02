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


#include "RenderManagerEvents.h"
#include "RenderCamera.h"
#include "RenderMemorizer.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"


namespace marengine {


	void FRenderManagerEvents::onVertexIndexBuffersUpdate(const FMeshBatchStatic& meshBatch) {
		const auto& pipeline{ PipelineStorage::Instance->getPipeline(meshBatch.getUniquePipelineID()) };
		pipeline.bind();
		pipeline.update(meshBatch.getVertices(), meshBatch.getIndices());
	}

	void FRenderManagerEvents::onTransformsUpdate(const FMeshBatchStatic& meshBatch) {
		const FTransformsArray& transforms{ meshBatch.getTransforms() };
		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(meshBatch.getTransformsSSBOindex()) };
		transformsSSBO.bind();
		transformsSSBO.update<float>(GLSL_SSBOs::ut_u_SeparateTransform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
	}

	void FRenderManagerEvents::onColorsUpdate(const FMeshBatchStaticColor& meshColorBatch) {
		const FColorsArray& colors{ meshColorBatch.getColors() };
		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(meshColorBatch.getColorsSSBOindex()) };
		colorsSSBO.bind();
		colorsSSBO.update<float>(GLSL_SSBOs::ut_u_Color.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
	}

	void FRenderManagerEvents::onRenderCameraUpdate(const RenderCamera* renderCamera) {
		const auto& cameraMVP{ renderCamera->getMVP() };
		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSL_SSBOs::ut_u_MVP.offset, sizeof(maths::mat4), maths::mat4::value_ptr(cameraMVP));
	}

	void FRenderManagerEvents::onPointLightUpdate(const FPointLightBatch& pointLightBatch) {
		const FPointLightsArray& pointLights{ pointLightBatch.getLights() };
		const int32_t lightSize{ (int32_t)pointLights.size() };
		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(pointLightBatch.getPointLightSSBOindex()) };
		pointLightSSBO.bind();
		pointLightSSBO.update<float>(GLSL_SSBOs::ut_u_material.offset, sizeof(LightMaterial) * pointLights.size(), &pointLights[0].position.x);
		pointLightSSBO.update<int32_t>(GLSL_SSBOs::ut_u_lightSize.offset, sizeof(int32_t), &lightSize);
	}


}