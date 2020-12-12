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
#include "../../ecs/Components/Components.h"
#include "../GraphicsLogs.h"
#include "ShaderBufferStorage.h"
#include "../../../Platform/OpenGL/ShaderUniforms.h"


namespace mar::graphics {


	RenderEvents RenderEvents::s_instance;

	const RenderEvents& RenderEvents::Instance() { 
		return s_instance; 
	}

	void RenderEvents::onDrawCall() const {
		RenderStatistics::Instance->drawCallsCount += 1;
	
		GRAPHICS_TRACE("RENDER_EVENTS: Handling draw call");
	}

	void RenderEvents::onTransformMat4Update(const ecs::TransformComponent& transform, const ecs::RenderPipelineComponent& rpc) const {
		if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
			RenderPipeline::Instance->m_containers2D[rpc.containerIndex].m_transforms[rpc.transformIndex] = transform.getTransform();
		}
		else if (rpc.materialType == (size_t)MaterialRenderType::CUBEMAP) {
			RenderPipeline::Instance->m_containersCubemap[rpc.containerIndex].m_transforms[rpc.transformIndex] = transform.getTransform();
		}
	}
	
	void RenderEvents::onLightUpdate(maths::vec3 position, const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc) const{
		onLightPositionUpdate(position, rpc);
		onLightComponentUpdate(light, rpc);
	}

	void RenderEvents::onLightPositionUpdate(maths::vec3 position, const ecs::RenderPipelineComponent& rpc) const {
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

	void RenderEvents::onColorUpdate(maths::vec4 color, const ecs::RenderPipelineComponent& rpc) const {
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
		GRAPHICS_INFO("RENDERER_BATCH: passing camera data to shader!");

		using namespace platforms::ShaderUniforms;
		using namespace mar::maths;

		const auto& cameraShaderBuffer = ShaderBufferStorage::Instance()->getCorrectShaderBuffer(ub_Camera);

		cameraShaderBuffer.bind();

		cameraShaderBuffer.update<float>(ut_u_CameraPos.offset, sizeof(vec3), vec3::value_ptr(camera.getPosition()));
		cameraShaderBuffer.update<float>(ut_u_Model.offset, sizeof(mat4), mat4::value_ptr(camera.getModel()));
		cameraShaderBuffer.update<float>(ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));

		GRAPHICS_INFO("RENDERER_BATCH: passed camera to shader!");
	}


}
