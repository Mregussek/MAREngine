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


#include "EventsLightBatch.h"
#include"../RenderAPI/RenderBufferManager.h"
#include "../RenderAPI/RenderPipeline.h"
#include "../../ecs/Entity/Entity.h"
#include "../../ecs/Components/Components.h"


namespace marengine {


	void FEventsLightBatch::onPointLightUpdate(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		auto& pointLightComponent{ entity.getComponent<PointLightComponent>() };
		const auto& lightBatchInfoComponent{ entity.getComponent<LightBatchInfoComponent>() };

		auto& pointLightAtBatch{ RenderPipeline::Instance->m_pointLightBatch.m_lights[lightBatchInfoComponent.indexAtBatch] };

		pointLightComponent.pointLight.position = maths::vec4(transformComponent.position, 1.f);
		pointLightAtBatch = pointLightComponent.pointLight;

		FRenderBufferManager::onPointLightUpdate(RenderPipeline::Instance->m_pointLightBatch);
	}

	void FEventsLightBatch::onPointLightPositionUpdate(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& lightBatchInfoComponent{ entity.getComponent<LightBatchInfoComponent>() };
		auto& pointLightAtBatch{ RenderPipeline::Instance->m_pointLightBatch.m_lights[lightBatchInfoComponent.indexAtBatch] };

		pointLightAtBatch.position = maths::vec4(transformComponent.position, 1.f);

		FRenderBufferManager::onPointLightUpdate(RenderPipeline::Instance->m_pointLightBatch);
	}


}
