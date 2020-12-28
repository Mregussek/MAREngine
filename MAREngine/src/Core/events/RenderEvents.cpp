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

		auto& containersColor{ RenderPipeline::Instance->m_containersColor };
		auto& containers2D{ RenderPipeline::Instance->m_containers2D };
		auto& containersCubemap{ RenderPipeline::Instance->m_containersCubemap };

		auto createPipelineStorage = [](RenderContainer& container) {
			auto& pipeline = PipelineStorage::Instance->createPipeline();
			pipeline.initialize(GraphicLimits::sizeOfVertices, GraphicLimits::sizeOfIndices);
			pipeline.bind();
			pipeline.update(container.getVertices(), container.getIndices());
			PipelineStorage::Instance->fillContainer(container);
		};
		
		std::for_each(containersColor.begin(), containersColor.end(), createPipelineStorage);
		std::for_each(containers2D.begin(), containers2D.end(), createPipelineStorage);
		std::for_each(containersCubemap.begin(), containersCubemap.end(), createPipelineStorage);
	}

	void RenderEvents::onTransformMat4Update(const TransformComponent& transform, const RenderPipelineComponent& rpc) const {
		auto getTransformFromCorrectContainer = [&rpc]()->maths::mat4& {
			if (rpc.materialType == (size_t)MaterialRenderType::COLOR) {
				return RenderPipeline::Instance->m_containersColor[rpc.containerIndex].m_transforms[rpc.transformIndex];
			}
			else if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
				return RenderPipeline::Instance->m_containers2D[rpc.containerIndex].m_transforms[rpc.transformIndex];
			}
			else if (rpc.materialType == (size_t)MaterialRenderType::CUBEMAP) {
				return RenderPipeline::Instance->m_containersCubemap[rpc.containerIndex].m_transforms[rpc.transformIndex];
			}
		};

		auto& containerTransform{ getTransformFromCorrectContainer() };
		containerTransform = transform.getTransform();
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
		auto getColorFromCorrectContainer = [&rpc]()->maths::vec4& {
			if (rpc.materialType == (size_t)MaterialRenderType::COLOR) {
				return RenderPipeline::Instance->m_containersColor[rpc.containerIndex].m_colors[rpc.colorIndex].second;
			}
			else if (rpc.materialType == (size_t)MaterialRenderType::TEXTURE2D) {
				return RenderPipeline::Instance->m_containers2D[rpc.containerIndex].m_colors[rpc.colorIndex].second;
			}
			else if (rpc.materialType == (size_t)MaterialRenderType::CUBEMAP) {
				return RenderPipeline::Instance->m_containersCubemap[rpc.containerIndex].m_colors[rpc.colorIndex].second;
			}
		};

		auto& containerColor{ getColorFromCorrectContainer() };
		containerColor = color;
	}
	
	void RenderEvents::onMainCameraUpdate(const RenderCamera& camera) const {
		passCameraToSSBO(camera);
	}

	void RenderEvents::passCameraToSSBO(const RenderCamera& camera) const {
		GRAPHICS_INFO("RENDER_EVENTS: passing camera data to ssbo!");

		auto& cameraSSBO{ ShaderBufferStorage::Instance->getSSBO(RenderMemorizer::Instance->cameraSSBO) };

		cameraSSBO.bind();
		cameraSSBO.update<float>(GLSL_SSBOs::ut_u_MVP.offset, sizeof(mat4), mat4::value_ptr(camera.getMVP()));
	}


}