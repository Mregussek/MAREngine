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


#ifndef MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H
#define MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H


namespace marengine {

	class Entity;


	/**
	 * @class FEventsLightBatch EventsLightBatch.h "Core/graphics/Lightning/EventsLightBatch.h"
	 * @brief Class describes events, that entity, with assigned some light component to it, can encounter.
	 * Entity, that has updated its light data, informs specific MAREngine's parts.
	 */
	class FEventsLightBatch {
	public:

		/**
		 * @brief Event should be called, when pointLight has updated. This should be called 
		 * when some FPointLight parameters has changed and everything Render state 
		 * should be informed. Updates also position.
		 * @param entity entity with PointLightComponent updated
		 */
		static void onPointLightUpdate(const Entity& entity);

		/**
		 * @brief Event should be called, only when position of pointLight has updated. That is
		 * FPointLight parameters are the same, but entity's position in TransformComponent has changed.
		 * If entity position changes, then PointLightComponent's position will also change.
		 * @param entity entity with position updated
		 */
		static void onPointLightPositionUpdate(const Entity& entity);

	};


}


#endif // !MAR_ENGINE_F_EVENTS_LIGHT_BATCH_H
