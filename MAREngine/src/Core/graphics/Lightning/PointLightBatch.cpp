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
#include "../IRender.h"
#include "../../ecs/Entity/Components.h"
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
		const bool thereIsPlaceInBatch{ currentLightSize + 1 < GraphicLimits::maxLights };

		if (thereIsPlaceInBatch && entityWithLight.hasComponent<CPointLight>()) {
			return true;
		}
		else {
			return false;
		}
	}

	void FPointLightBatch::submitEntityWithLightning(const Entity& entity) {
		const auto& cTransform{ entity.getComponent<CTransform>() };
		auto& cPointLight{ entity.getComponent<CPointLight>() };

		FPointLight& pointLightData{ m_lights.emplace_back(cPointLight.pointLight) };
		pointLightData.position = maths::vec4(cTransform.position, 1.f);

        cPointLight.batch.index = (int8)(m_lights.size() - 1);
        cPointLight.batch.type = ELightBatchType::POINTLIGHT;
	}

	const FPointLightsArray& FPointLightBatch::getLights() const {
		return m_lights;
	}

	void FPointLightBatch::updateLight(const Entity& entity) {
	    const auto& cPointLight{ entity.getComponent<CPointLight>() };
	    m_lights.at(cPointLight.batch.index) = cPointLight.pointLight;
	}

	uint32_t FPointLightBatch::getUniquePointLightID() const {
		return m_uniquePointLightID;
	}

	void FPointLightBatch::setUniquePointLightID(uint32_t index) {
		m_uniquePointLightID = index;
	}

	ELightBatchType FPointLightBatch::getBatchType() const {
		return ELightBatchType::POINTLIGHT;
	}


}