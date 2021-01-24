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


#include "PointLightBatch.h"
#include "../GraphicsLimits.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Components/LightComponents.h"
#include "../../ecs/Entity/Entity.h"


namespace marengine {


	void FPointLightBatch::reset() {
		m_lights.clear();
	}

	bool FPointLightBatch::hasAnythingToDraw() const {
		return !m_lights.empty();
	}

	bool FPointLightBatch::canBeBatched(const Entity& entityWithLight) const {
		const auto currentLightSize{ m_lights.size() };
		const bool thereIsPlaceInBatch{ !(currentLightSize + 1 >= GraphicLimits::maxLights) };

		if (thereIsPlaceInBatch && entityWithLight.hasComponent<PointLightComponent>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FPointLightBatch::submitEntityWithLightning(const Entity& entity) {
		GRAPHICS_TRACE("F_POINT_LIGHT_BATCH: submitting {} entity with point light...", entity.getComponent<TagComponent>().tag);

		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& pointLightComponent{ entity.getComponent<PointLightComponent>() };

		FPointLight& pointLightData{ m_lights.emplace_back(pointLightComponent.pointLight) };
		pointLightData.position = maths::vec4(transformComponent.position, 1.f);

		auto& rpc{ entity.getComponent<RenderPipelineComponent>() };
		rpc.lightIndex = m_lights.size() - 1;

		GRAPHICS_DEBUG("F_POINT_LIGHT_BATCH: submitted {} entity with pointLight, current lights size = {},"
			"assigned rpc.lightIndex {}",
			entity.getComponent<TagComponent>().tag, m_lights.size(), rpc.lightIndex);
	}

	const FPointLightsArray& FPointLightBatch::getLights() const {
		return m_lights;
	}

	uint32_t FPointLightBatch::getUniquePointLightID() const {
		return m_uniquePointLightID;
	}

	void FPointLightBatch::setUniquePointLightID(uint32_t index) {
		m_uniquePointLightID = index;
	}


}