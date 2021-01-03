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
#include "../GraphicsLogs.h"
#include "../Mesh/MeshBatchStaticColor.h"
#include "../Mesh/MeshBatchStaticTexture2D.h"
#include "../Lightning/PointLightBatch.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FRenderManagerEvents::onVertexIndexBuffersUpdate(const FMeshBatchStatic& meshBatch) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onVertexIndexBuffersUpdate");

		const uint32_t uniqueID{ meshBatch.getUniquePipelineID() };
		const auto& pipeline{ PipelineStorage::Instance->getPipeline(uniqueID) };
		pipeline.bind();
		pipeline.update(meshBatch.getVertices(), meshBatch.getIndices());

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onVertexIndexBuffersUpdate, uniquePipelineID = {}", uniqueID);
	}

	void FRenderManagerEvents::onTransformsUpdate(const FMeshBatchStatic& meshBatch) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onTransformsUpdate");

		const FTransformsArray& transforms{ meshBatch.getTransforms() };
		const uint32_t uniqueID{ meshBatch.getTransformsSSBOindex() };

		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		transformsSSBO.bind();
		transformsSSBO.update<float>(GLSLShaderInfo::Transform.offset, transforms.size() * sizeof(maths::mat4), maths::mat4::value_ptr(transforms));

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onTransformsUpdate, uniqueTransformsSSBO index = {}", uniqueID);
	}

	void FRenderManagerEvents::onColorsUpdate(const FMeshBatchStaticColor& meshColorBatch) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onColorsUpdate");

		const FColorsArray& colors{ meshColorBatch.getColors() };
		const uint32_t uniqueID{ meshColorBatch.getColorsSSBOindex() };

		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		colorsSSBO.bind();
		colorsSSBO.update<float>(GLSLShaderInfo::Colors.offset, colors.size() * sizeof(maths::vec4), maths::vec4::value_ptr(colors));

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onColorsUpdate, uniqueColorsSSBO index = {}", uniqueID);
	}

	void FRenderManagerEvents::onRenderCameraUpdate(const RenderCamera* renderCamera) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onRenderCameraUpdate");

		const auto& cameraMVP{ renderCamera->getMVP() };
		const uint32_t uniqueID{ RenderMemorizer::Instance->cameraSSBO };

		const auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSLShaderInfo::MVP.offset, sizeof(maths::mat4), maths::mat4::value_ptr(cameraMVP));

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onRenderCameraUpdate, uniqueCameraSSBO index = {}", uniqueID);
	}

	void FRenderManagerEvents::onPointLightUpdate(const FPointLightBatch& pointLightBatch) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onPointLightUpdate");

		const FPointLightsArray& pointLights{ pointLightBatch.getLights() };
		const int32_t lightSize{ (int32_t)pointLights.size() };
		const uint32_t uniqueID{ pointLightBatch.getPointLightSSBOindex() };

		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		pointLightSSBO.bind();
		pointLightSSBO.update<float>(GLSLShaderInfo::LightMaterial.offset, sizeof(LightMaterial) * pointLights.size(), &pointLights[0].position.x);
		pointLightSSBO.update<int32_t>(GLSLShaderInfo::LightMaterialSize.offset, sizeof(int32_t), &lightSize);

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onPointLightUpdate, uniquePointLightsSSBO index = {}", uniqueID);
	}

	void FRenderManagerEvents::onTransformAtMeshUpdate(const Entity& entity) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onTransformAtMeshUpdate, updated {} entity", entity.getComponent<TagComponent>().tag);

		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };
		maths::mat4& transformAtMesh{ meshBatchComponent.assignedMesh->p_transforms[meshBatchComponent.transformIndex] };

		transformAtMesh = transformComponent.getTransform();

		const uint32_t specificTransformOffset{ meshBatchComponent.transformIndex * sizeof(maths::mat4) };
		const uint32_t offset{ GLSLShaderInfo::Transform.offset + specificTransformOffset };
		const uint32_t uniqueID{ meshBatchComponent.assignedMesh->getTransformsSSBOindex() };
		const float* pPtrToTransform{ maths::mat4::value_ptr(transformAtMesh) };

		const auto& transformsSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		transformsSSBO.bind();
		transformsSSBO.update<float>(offset, sizeof(maths::mat4), pPtrToTransform);

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onTransformAtMeshUpdate, entity {}, uniqueTransformsSSBO index = {}, meshBatchComponent.transformIndex = {}", 
			entity.getComponent<TagComponent>().tag, uniqueID, meshBatchComponent.transformIndex);
	}

	void FRenderManagerEvents::onColorAtMeshUpdate(const Entity& entity) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onColorAtMeshUpdate, updated {} entity", entity.getComponent<TagComponent>().tag);

		const auto& colorComponent{ entity.getComponent<ColorComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };

		FMeshBatchStaticColor* meshColorBatch{ (FMeshBatchStaticColor*)meshBatchComponent.assignedMesh };
		maths::vec4& colorAtMesh{ meshColorBatch->m_colors[meshBatchComponent.materialIndex] };

		colorAtMesh = colorComponent.texture;

		const uint32_t specificColorOffset{ meshBatchComponent.materialIndex * sizeof(maths::vec4) };
		const uint32_t offset{ GLSLShaderInfo::Colors.offset + specificColorOffset };
		const uint32_t uniqueID{ meshColorBatch->getColorsSSBOindex() };
		const float* pPointerToColor{ maths::vec4::value_ptr(colorAtMesh) };

		const auto& colorsSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		colorsSSBO.bind();
		colorsSSBO.update<float>(offset, sizeof(maths::vec4), pPointerToColor);

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onColorAtMeshUpdate, entity {}, uniqueColorsSSBO index = {}, meshBatchComponent.materialIndex = {}",
			entity.getComponent<TagComponent>().tag, uniqueID, meshBatchComponent.materialIndex);
	}

	void FRenderManagerEvents::onTexture2DatMeshUpdate(const Entity& entity) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onTexture2DatMeshUpdate, updated {} entity", entity.getComponent<TagComponent>().tag);

		const auto& texture2DComponent{ entity.getComponent<Texture2DComponent>() };
		const auto& meshBatchComponent{ entity.getComponent<MeshBatchComponent>() };

		FMeshBatchStaticTexture2D* meshColorBatch{ (FMeshBatchStaticTexture2D*)meshBatchComponent.assignedMesh };
		std::string& instanceAtAssignedMesh{ meshColorBatch->m_textures[meshBatchComponent.materialIndex].texturePath };

		instanceAtAssignedMesh = texture2DComponent.texture;

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onTexture2DatMeshUpdate, entity {}, meshBatchComponent.materialIndex = {}",
			entity.getComponent<TagComponent>().tag, meshBatchComponent.materialIndex);
	}

	void FRenderManagerEvents::onPointLightAtBatchUpdate(const Entity& entity) {
		GRAPHICS_TRACE("F_RENDER_MANAGER_EVENTS: onPointLightAtBatchUpdate, updated {} entity", entity.getComponent<TagComponent>().tag);

		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& pointLightComponent{ entity.getComponent<LightComponent>() };
		const auto& lightBatchComponent{ entity.getComponent<LightBatchComponent>() };

		LightMaterial& lightAtBatch{ FRenderManager::Instance->m_pointLightBatch.m_lights[lightBatchComponent.pointLightIndex] };

		lightAtBatch.position = maths::vec4(transformComponent.center, 1.f);
		lightAtBatch.ambient = pointLightComponent.ambient;
		lightAtBatch.diffuse = pointLightComponent.diffuse;
		lightAtBatch.specular = pointLightComponent.specular;
		lightAtBatch.linear = pointLightComponent.linear;
		lightAtBatch.quadratic = pointLightComponent.quadratic;
		lightAtBatch.constant = pointLightComponent.constant;
		lightAtBatch.shininess = pointLightComponent.shininess;

		const uint32_t specificLightOffset{ lightBatchComponent.pointLightIndex * sizeof(LightMaterial) };
		const uint32_t offset{ GLSLShaderInfo::LightMaterial.offset + specificLightOffset };
		const uint32_t uniqueID{ FRenderManager::Instance->m_pointLightBatch.getPointLightSSBOindex() };
		const float* pPtrToPointLight{ &lightAtBatch.position.x };

		const auto& pointLightSSBO{ ShaderBufferStorage::Instance->getSSBO(uniqueID) };
		pointLightSSBO.bind();
		pointLightSSBO.update<float>(offset, sizeof(LightMaterial), pPtrToPointLight);

		GRAPHICS_DEBUG("F_RENDER_MANAGER_EVENTS: onPointLightAtBatchUpdate, entity {}, uniquePointLightsSSBO index = {}, lightBatchComponent.pointLightIndex = {}",
			entity.getComponent<TagComponent>().tag, uniqueID, lightBatchComponent.pointLightIndex);
	}


}