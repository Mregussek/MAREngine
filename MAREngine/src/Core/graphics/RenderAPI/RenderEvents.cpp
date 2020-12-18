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
#include "RenderCamera.h"
#include "RenderPipeline.h"
#include "RenderStatistics.h"
#include "../GraphicsLogs.h"
#include "../GraphicLimits.h"
#include "../Renderer/RenderMemorizer.h"
#include "../Renderer/PipelineStorage.h"
#include "../Renderer/ShaderBufferStorage.h"
#include "../../ecs/Components/Components.h"
#include "../../../Platform/GLSL/ShaderUniforms.h"


namespace mar::graphics {


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

		auto& containers2D{ RenderPipeline::Instance->m_containers2D };
		auto& containersCubemap{ RenderPipeline::Instance->m_containersCubemap };

		auto createPipelineStorage = [](RenderContainer& container) {
			auto& pipeline = PipelineStorage::Instance->createPipeline();
			pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);
			pipeline.bind();
			pipeline.update(container.getVertices(), container.getIndices());
			PipelineStorage::Instance->fillContainer(container);
		};
		
		std::for_each(containers2D.begin(), containers2D.end(), createPipelineStorage);
		std::for_each(containersCubemap.begin(), containersCubemap.end(), createPipelineStorage);
	}

	void RenderEvents::onTransformMat4Update(const ecs::TransformComponent& transform, const ecs::RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
			RenderPipeline::Instance->m_containers2D[rpc.containerIndex].m_transforms[rpc.transformIndex] = transform.getTransform();
		}
		else if (rpc.materialType == (size_t)MaterialRenderType::CUBEMAP) {
			RenderPipeline::Instance->m_containersCubemap[rpc.containerIndex].m_transforms[rpc.transformIndex] = transform.getTransform();
		}
	}
	
	void RenderEvents::onLightUpdate(vec3 position, const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc) const{
		onLightPositionUpdate(position, rpc);
		onLightComponentUpdate(light, rpc);
	}

	void RenderEvents::onLightPositionUpdate(vec3 position, const ecs::RenderPipelineComponent& rpc) const {
		RenderPipeline::Instance->m_lights[rpc.containerLightIndex].m_lightMaterials[rpc.lightIndex].position = maths::vec4(position, 1.f);
	}

	void RenderEvents::onLightComponentUpdate(const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc) const {
		auto& lightMaterial = RenderPipeline::Instance->m_lights[rpc.containerLightIndex].m_lightMaterials[rpc.lightIndex];

		lightMaterial.ambient = light.ambient;
		lightMaterial.diffuse = light.diffuse;
		lightMaterial.specular = light.specular;
		lightMaterial.linear = light.linear;
		lightMaterial.quadratic = light.quadratic;
		lightMaterial.constant = light.constant;
		lightMaterial.shininess = light.shininess;
	}

	void RenderEvents::onColorUpdate(vec4 color, const ecs::RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
			RenderPipeline::Instance->m_containers2D[rpc.containerIndex].m_colors[rpc.colorIndex].second = color;
		}
		else if (rpc.materialType == (size_t)MaterialRenderType::CUBEMAP) {
			RenderPipeline::Instance->m_containersCubemap[rpc.containerIndex].m_colors[rpc.colorIndex].second = color;
		}
	}
	
	void RenderEvents::onMainCameraUpdate(const RenderCamera& camera) const {
		passCameraToSSBO(camera);
	}

	void RenderEvents::passCameraToSSBO(const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDER_EVENTS: passing camera data to ssbo!");

		auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(SSBOsGL::ut_u_CameraPos.offset, sizeof(vec3), vec3::value_ptr(camera.getPosition()));
		cameraSSBO.update<float>(SSBOsGL::ut_u_Model.offset, sizeof(mat4), mat4::value_ptr(camera.getModel()));
		cameraSSBO.update<float>(SSBOsGL::ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));
	}


}
