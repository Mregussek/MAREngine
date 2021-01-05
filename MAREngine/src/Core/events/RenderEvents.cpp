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
#include "../graphics/RenderAPI/RenderCamera.h"
#include "../graphics/RenderAPI/RenderPipeline.h"
#include "../graphics/RenderAPI/RenderStatistics.h"
#include "../graphics/GraphicsLogs.h"
#include "../graphics/GraphicLimits.h"
#include "../graphics/Renderer/RenderMemorizer.h"
#include "../graphics/Renderer/PipelineStorage.h"
#include "../graphics/Renderer/ShaderBufferStorage.h"
#include "../ecs/Components/Components.h"
#include "../../Platform/GLSL/ShaderUniforms.h"


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

		auto& staticColorBatches{ RenderPipeline::Instance->m_staticColorBatches };
		auto& staticTexture2DBatches{ RenderPipeline::Instance->m_staticTexture2DBatches };

		auto createPipelineStorage = [](FMeshBatchStatic& batch) {
			auto& pipeline = PipelineStorage::Instance->createPipeline();
			pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);
			pipeline.bind();
			pipeline.update(batch.getVertices(), batch.getIndices());
			batch.setUniquePipelineID(PipelineStorage::Instance->getPipelines().size() - 1);
		};
		
		std::for_each(staticColorBatches.begin(), staticColorBatches.end(), createPipelineStorage);
		std::for_each(staticTexture2DBatches.begin(), staticTexture2DBatches.end(), createPipelineStorage);
	}

	void RenderEvents::onTransformMat4Update(const TransformComponent& transform, const RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)MaterialRenderType::COLOR) {
			RenderPipeline::Instance->m_staticColorBatches[rpc.containerIndex].p_transforms[rpc.transformIndex] = transform.getTransform();
		}
		else if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
			RenderPipeline::Instance->m_staticTexture2DBatches[rpc.containerIndex].p_transforms[rpc.transformIndex] = transform.getTransform();
		}
	}
	
	void RenderEvents::onLightUpdate(vec3 position, const LightComponent& light, const RenderPipelineComponent& rpc) const{
		onLightPositionUpdate(position, rpc);
		onLightComponentUpdate(light, rpc);
	}

	void RenderEvents::onLightPositionUpdate(vec3 position, const RenderPipelineComponent& rpc) const {
		RenderPipeline::Instance->m_lights[rpc.containerLightIndex].m_lightMaterials[rpc.lightIndex].position = maths::vec4(position, 1.f);
	}

	void RenderEvents::onLightComponentUpdate(const LightComponent& light, const RenderPipelineComponent& rpc) const {
		auto& lightMaterial = RenderPipeline::Instance->m_lights[rpc.containerLightIndex].m_lightMaterials[rpc.lightIndex];

		lightMaterial.ambient = light.ambient;
		lightMaterial.diffuse = light.diffuse;
		lightMaterial.specular = light.specular;
		lightMaterial.linear = light.linear;
		lightMaterial.quadratic = light.quadratic;
		lightMaterial.constant = light.constant;
		lightMaterial.shininess = light.shininess;
	}

	void RenderEvents::onColorUpdate(vec4 color, const RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)MaterialRenderType::COLOR) {
			RenderPipeline::Instance->m_staticColorBatches[rpc.containerIndex].m_colors[rpc.colorIndex] = color;
		}
	}
	
	void RenderEvents::onMainCameraUpdate(const RenderCamera& camera) const {
		passCameraToSSBO(camera);
	}

	void RenderEvents::passCameraToSSBO(const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDER_EVENTS: passing camera data to ssbo!");

		auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSLShaderInfo::MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));
	}


}
