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
#include "RenderPipeline.h"
#include "../../ecs/Components/Components.h"
#include "../GraphicsLogs.h"


namespace mar::graphics {


	RenderEvents RenderEvents::s_instance;

	void RenderEvents::setRenderPipeline(RenderPipeline& renderPipeline) {
		m_renderPipeline = &renderPipeline;
	}

	void RenderEvents::onDrawCall() {
		auto& stats = m_renderPipeline->getStatistics();
		stats.drawCallsCount += 1;
	
		GRAPHICS_TRACE("RENDER_EVENTS: Handling draw call");
	}

	void RenderEvents::onTransformMat4Update(const maths::mat4& transform, const ecs::RenderPipelineComponent& rpc) {
		m_renderPipeline->m_containers[rpc.containerIndex].m_transforms[rpc.transformIndex] = transform;
	}
	
	void RenderEvents::onLightUpdate(maths::vec3 position, const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc) {
		onLightPositionUpdate(position, rpc);
		onLightComponentUpdate(light, rpc);
	}

	void RenderEvents::onLightPositionUpdate(maths::vec3 position, const ecs::RenderPipelineComponent& rpc) {
		m_renderPipeline->m_containers[rpc.containerLightIndex].m_lights[rpc.lightIndex].position = position;
	}

	void RenderEvents::onLightComponentUpdate(const ecs::LightComponent& light, const ecs::RenderPipelineComponent& rpc) {
		auto& lightMaterial = m_renderPipeline->m_containers[rpc.containerLightIndex].m_lights[rpc.lightIndex];

		lightMaterial.ambient = light.ambient;
		lightMaterial.diffuse = light.diffuse;
		lightMaterial.specular = light.specular;
		lightMaterial.linear = light.linear;
		lightMaterial.quadratic = light.quadratic;
		lightMaterial.constant = light.constant;
		lightMaterial.shininess = light.shininess;
	}

	void RenderEvents::onColorUpdate(maths::vec3 color, const ecs::RenderPipelineComponent& rpc) {
		m_renderPipeline->m_containers[rpc.containerIndex].m_colors[rpc.colorIndex].second = color;
	}
	

}
