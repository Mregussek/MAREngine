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


#ifndef MAR_ENGINE_F_POINT_LIGHT_BATCH_H
#define MAR_ENGINE_F_POINT_LIGHT_BATCH_H


#include "LightDefinitions.h"


namespace marengine {

	class Entity;


	class FPointLightBatch {

		friend class FEventsLightBatch;

	public:

		void reset();

		bool hasAnythingToDraw() const;

		bool canBeBatched(const Entity& entityWithLight) const;
		void submitEntityWithLightning(const Entity& entity);

		const FPointLightsArray& getLights() const;

		uint32_t getUniquePointLightID() const;
		void setUniquePointLightID(uint32_t index);

		ELightBatchType getBatchType() const;

	private:

		FPointLightsArray m_lights;
		uint32_t m_uniquePointLightID{ 0 };

	};
	

}



#endif // !MAR_ENGINE_F_POINT_LIGHT_BATCH_H
