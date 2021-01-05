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


#include "RenderEvents.h"
#include "../graphics/Mesh/MeshBatchStatic.h"
#include "../graphics/RenderAPI/RenderCamera.h"
#include "../graphics/RenderAPI/RenderPipeline.h"
#include "../graphics/RenderAPI/RenderStatistics.h"
#include "../graphics/GraphicsLogs.h"
#include "../graphics/GraphicsLimits.h"
#include "../graphics/Renderer/RenderMemorizer.h"
#include "../graphics/Renderer/PipelineStorage.h"
#include "../graphics/Renderer/ShaderBufferStorage.h"
#include "../ecs/Components/Components.h"
#include "../../Platform/GLSL/ShaderUniforms.h"
#include "../graphics/RenderAPI/RenderBufferManager.h"


namespace marengine {


	RenderEvents RenderEvents::s_instance;

	const RenderEvents& RenderEvents::Instance() { 
		return s_instance; 
	}

	void RenderEvents::onDrawCall() const {
		RenderStatistics::Instance->drawCallsCount += 1;
	
		GRAPHICS_TRACE("RENDER_EVENTS: Handling draw call");
	}

	void RenderEvents::onContainersReadyToDraw() const {
		PipelineStorage::Instance->close();
		ShaderBufferStorage::Instance->close();

		FRenderBufferManager::onMeshBatchReadyToDraw(RenderPipeline::Instance->m_staticColorBatches);
		FRenderBufferManager::onMeshBatchReadyToDraw(RenderPipeline::Instance->m_staticTexture2DBatches);
		FRenderBufferManager::onLightBatchReadyToDraw(RenderPipeline::Instance->m_pointLightBatches);
		FRenderBufferManager::onRenderCameraReadyToDraw(RenderPipeline::Instance->m_camera);
	}

	void RenderEvents::onTransformMat4Update(const TransformComponent& transform, const RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)EMeshBatchStaticType::COLOR) {
			auto& batch{ RenderPipeline::Instance->m_staticColorBatches[rpc.containerIndex] };
			batch.p_transforms[rpc.transformIndex] = transform.getTransform();
			FRenderBufferManager::onTransformsUpdate(batch);
		}
		else if (rpc.materialType == (size_t)EMeshBatchStaticType::TEXTURE2D) {
			auto& batch{ RenderPipeline::Instance->m_staticTexture2DBatches[rpc.containerIndex] };
			batch.p_transforms[rpc.transformIndex] = transform.getTransform();
			FRenderBufferManager::onTransformsUpdate(batch);
		}
	}
	
	void RenderEvents::onLightUpdate(maths::vec3 position, const LightComponent& light, const RenderPipelineComponent& rpc) const{
		onLightPositionUpdate(position, rpc);
		onLightComponentUpdate(light, rpc);
	}

	void RenderEvents::onLightPositionUpdate(maths::vec3 position, const RenderPipelineComponent& rpc) const {
		auto& batch{ RenderPipeline::Instance->m_pointLightBatches[rpc.containerLightIndex] };
		batch.m_lights[rpc.lightIndex].position = maths::vec4(position, 1.f);
		FRenderBufferManager::onLightUpdate(batch);
	}

	void RenderEvents::onLightComponentUpdate(const LightComponent& light, const RenderPipelineComponent& rpc) const {
		auto& batch{ RenderPipeline::Instance->m_pointLightBatches[rpc.containerLightIndex] };
		auto& pointLight{ batch.m_lights[rpc.lightIndex] };

		pointLight.ambient = light.ambient;
		pointLight.diffuse = light.diffuse;
		pointLight.specular = light.specular;
		pointLight.linear = light.linear;
		pointLight.quadratic = light.quadratic;
		pointLight.constant = light.constant;
		pointLight.shininess = light.shininess;

		FRenderBufferManager::onLightUpdate(batch);
	}

	void RenderEvents::onColorUpdate(maths::vec4 color, const RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)EMeshBatchStaticType::COLOR) {
			auto& batch{ RenderPipeline::Instance->m_staticColorBatches[rpc.containerIndex] };
			batch.m_colors[rpc.colorIndex] = color;
			FRenderBufferManager::onColorUpdate(batch);
		}
	}
	
	void RenderEvents::onMainCameraUpdate(const RenderCamera& camera) const {
		passCameraToSSBO(camera);
	}

	void RenderEvents::passCameraToSSBO(const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDER_EVENTS: passing camera data to ssbo!");

		FRenderBufferManager::onRenderCameraUpdate(&camera);
	}


}
