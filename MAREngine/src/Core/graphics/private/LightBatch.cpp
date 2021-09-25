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


#include "../public/LightBatch.h"
#include "../public/IRender.h"
#include "../../ecs/Entity/Components.h"
#include "../../ecs/Entity/Entity.h"
#include "../../../Logging/Logger.h"


namespace marengine {


    int32 FLightBatch::getLightSSBO() const {
        return p_lightSSBO;
    }

    void FLightBatch::passLightSSBO(int32 index) {
        p_lightSSBO = index;
    }


	void FPointLightBatch::reset() {
		m_lights.clear();
	}

    const float* FPointLightBatch::getBatchData() const {
        if(!m_lights.empty()) {
            return &m_lights.at(0).position.x;
        }

        return nullptr;
    }

    uint32 FPointLightBatch::getCountLight() const {
        return m_lights.size();
    }

    bool FPointLightBatch::shouldBeBatched(const Entity& entity) const {
        const std::string& entityTag{ entity.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Checking, if entity {} should be batched at PointLightBatch", entityTag);
        if(entity.hasComponent<CPointLight>()) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} should be batched at PointLightBatch", entityTag);
            return true;
        }

        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} should not be batched at PointLightBatch", entityTag);
        return false;
    }

	bool FPointLightBatch::canBeBatched(const Entity& entityWithLight) const {
        const std::string& entityTag{ entityWithLight.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Checking, if entity {} can be batched in this batch...", entityTag);

		const auto currentLightSize{ m_lights.size() };
		const bool thereIsPlaceInBatch{ currentLightSize + 1 < GraphicLimits::maxLights };

		if (thereIsPlaceInBatch && entityWithLight.hasComponent<CPointLight>()) {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} can be batched...", entityTag);
			return true;
		}
		else {
            MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} cannot be batched...", entityTag);
			return false;
		}
	}

	void FPointLightBatch::submitToBatch(const Entity& entity) {
        const std::string& entityTag{ entity.getComponent<CTag>().tag };
        MARLOG_TRACE(ELoggerType::GRAPHICS, "Submitting entity {} to batch...", entityTag);

		const auto& cTransform{ entity.getComponent<CTransform>() };
		auto& cPointLight{ entity.getComponent<CPointLight>() };

		FPointLight& pointLightData{ m_lights.emplace_back(cPointLight.pointLight) };
		pointLightData.position = maths::vec4(cTransform.position, 1.f);

        cPointLight.batch.index = (int8)(m_lights.size() - 1);
        cPointLight.batch.type = ELightBatchType::POINTLIGHT;

        MARLOG_DEBUG(ELoggerType::GRAPHICS, "Entity {} submitted to batch!", entityTag);
	}

	void FPointLightBatch::updateLight(const Entity& entity) {
	    const auto& cPointLight{ entity.getComponent<CPointLight>() };
	    m_lights.at(cPointLight.batch.index) = cPointLight.pointLight;
	}

	ELightBatchType FPointLightBatch::getType() const {
		return ELightBatchType::POINTLIGHT;
	}


    void FLightBatchStorage::reset() {
        m_pointLightBatch.reset();
    }

    FPointLightBatch* FLightBatchStorage::getPointLightBatch() const {
        return const_cast<FPointLightBatch*>(&m_pointLightBatch);
    }

    uint32 FLightBatchStorage::getCountPointLightBatch() const {
        return 1;
    }


    FPointLightBatch* FLightBatchFactory::emplacePointLightBatch() {
        return m_storage.getPointLightBatch();
    }

    FLightBatchStorage* FLightBatchFactory::getStorage() const {
        return const_cast<FLightBatchStorage*>(&m_storage);
    }


}