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


#include "PointLightBatch.h"
#include "../GraphicsLimits.h"
#include "../GraphicsLogs.h"
#include "../../ecs/Components/LightComponents.h"
#include "../../ecs/Entity/Entity.h"


namespace marengine {


	void FPointLightBatch::reset() {
		m_lights.clear();
	}

	bool FPointLightBatch::canBeBatched(const Entity& entityWithLight) const {
		const auto currentLightSize{ m_lights.size() };
		const bool thereIsPlaceInBatch{ !(currentLightSize + 1 >= GraphicLimits::maxLights) };

		if (thereIsPlaceInBatch && entityWithLight.hasComponent<LightComponent>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FPointLightBatch::submitEntityWithLightning(const Entity& entity) {
		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		const auto& lightComponent{ entity.getComponent<LightComponent>() };

		LightMaterial& lightMaterial{ m_lights.emplace_back() };
		lightMaterial.position = maths::vec4(transformComponent.center, 1.f);
		lightMaterial.ambient = lightComponent.ambient;
		lightMaterial.diffuse = lightComponent.diffuse;
		lightMaterial.specular = lightComponent.specular;
		lightMaterial.linear = lightComponent.linear;
		lightMaterial.quadratic = lightComponent.quadratic;
		lightMaterial.constant = lightComponent.constant;
		lightMaterial.shininess = lightComponent.shininess;

		auto& lightBatchComponent{ entity.getComponent<LightBatchComponent>() };
		lightBatchComponent.pointLightIndex = m_lights.size() - 1;
	}

	const FPointLightsArray& FPointLightBatch::getLights() const {
		return m_lights;
	}

	uint32_t FPointLightBatch::getPointLightSSBOindex() const {
		return m_pointLightSSBOindex;
	}

	void FPointLightBatch::setPointLightSSBOindex(uint32_t index) {
		m_pointLightSSBOindex = index;
	}


}