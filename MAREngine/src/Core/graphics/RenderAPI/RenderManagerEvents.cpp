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
#include "RenderManager.h"
#include "RenderMemorizer.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


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
		transformsSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));
	}

	void FRenderManagerEvents::onColorsUpdate(const FMeshBatchStaticColor& meshColorBatch) {
		const FColorsArray& colors{ meshColorBatch.getColors() };
		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(meshColorBatch.getColorsSSBOindex()) };
		colorsSSBO.bind();
		colorsSSBO.update<float>(GLSLShaderInfo::Colors.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));
	}

	void FRenderManagerEvents::onRenderCameraUpdate(const RenderCamera* renderCamera) {
		const auto& cameraMVP{ renderCamera->getMVP() };
		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };
		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSLShaderInfo::MVP.offset, sizeof(maths::mat4), maths::mat4::value_ptr(cameraMVP));
	}

	void FRenderManagerEvents::onPointLightUpdate(const FPointLightBatch& pointLightBatch) {
		const FPointLightsArray& pointLights{ pointLightBatch.getLights() };
		const int32_t lightSize{ (int32_t)pointLights.size() };
		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(pointLightBatch.getPointLightSSBOindex()) };
		pointLightSSBO.bind();
		pointLightSSBO.update<float>(GLSLShaderInfo::LightMaterial.offset, sizeof(LightMaterial) * pointLights.size(), &pointLights[0].position.x);
		pointLightSSBO.update<int32_t>(GLSLShaderInfo::LightMaterialSize.offset, sizeof(int32_t), &lightSize);
	}

	void FRenderManagerEvents::onTransformAtMeshUpdate(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };
		maths::mat4& instanceAtAssignedMesh{ meshBatchComponent.assignedMesh->p_transforms[meshBatchComponent.transformIndex] };

		instanceAtAssignedMesh = transformComponent.getTransform();

		const uint32_t offset{ GLSLShaderInfo::Transform.offset + meshBatchComponent.transformIndex };
		const float* pPointerToInstance{ maths::mat4::value_ptr(instanceAtAssignedMesh) };

		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(meshBatchComponent.assignedMesh->getTransformsSSBOindex()) };
		transformsSSBO.bind();
		transformsSSBO.update<float>(offset, sizeof(maths::mat4), pPointerToInstance);
	}

	void FRenderManagerEvents::onColorAtMeshUpdate(const Entity& entity) {
		const auto& colorComponent{ entity.getComponent<ColorComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };

		FMeshBatchStaticColor* meshColorBatch{ (FMeshBatchStaticColor*)meshBatchComponent.assignedMesh };
		maths::vec4& instanceAtAssignedMesh{ meshColorBatch->m_colors[meshBatchComponent.materialIndex] };

		instanceAtAssignedMesh = colorComponent.texture;

		const uint32_t offset{ GLSLShaderInfo::Colors.offset + meshBatchComponent.materialIndex };
		const float* pPointerToInstance{ maths::vec4::value_ptr(instanceAtAssignedMesh) };

		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(meshColorBatch->getColorsSSBOindex()) };
		colorsSSBO.bind();
		colorsSSBO.update<float>(offset, sizeof(maths::vec4), pPointerToInstance);
	}

	void FRenderManagerEvents::onTexture2DatMeshUpdate(const Entity& entity) {
		const auto& texture2DComponent{ entity.getComponent<Texture2DComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };

		FMeshBatchStaticTexture2D* meshColorBatch{ (FMeshBatchStaticTexture2D*)meshBatchComponent.assignedMesh };
		std::string& instanceAtAssignedMesh{ meshColorBatch->m_textures[meshBatchComponent.materialIndex].texturePath };

		instanceAtAssignedMesh = texture2DComponent.texture;
	}

	void FRenderManagerEvents::onPointLightAtBatchUpdate(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& pointLightComponent{ entity.getComponent<LightComponent>() };
		const auto& lightBatchComponent{ entity.getComponent<LightBatchComponent>() };

		LightMaterial& lightInstanceAtBatch{ FRenderManager::Instance->m_pointLightBatch.m_lights[lightBatchComponent.pointLightIndex] };

		lightInstanceAtBatch.position = maths::vec4(transformComponent.center, 1.f);
		lightInstanceAtBatch.ambient = pointLightComponent.ambient;
		lightInstanceAtBatch.diffuse = pointLightComponent.diffuse;
		lightInstanceAtBatch.specular = pointLightComponent.specular;
		lightInstanceAtBatch.linear = pointLightComponent.linear;
		lightInstanceAtBatch.quadratic = pointLightComponent.quadratic;
		lightInstanceAtBatch.constant = pointLightComponent.constant;
		lightInstanceAtBatch.shininess = pointLightComponent.shininess;

		const uint32_t offset{ GLSLShaderInfo::LightMaterial.offset + lightBatchComponent.pointLightIndex };

		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(FRenderManager::Instance->m_pointLightBatch.getPointLightSSBOindex()) };
		pointLightSSBO.bind();
		pointLightSSBO.update<float>(offset, sizeof(LightMaterial), &lightInstanceAtBatch.position.x);
	}


}