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


#ifndef MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H
#define MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H


#include "../../mar.h"
#include "Components/Components.h"


namespace marengine {

	class Entity;


	/**
	 * @class FScenePlayStorage ScenePlayStorage.h "Core/ecs/ScenePlayStorage.h"
	 * @brief storage for play mode.
	 * Because we want to only test game in play mode and then return to its
	 * state afterwards we need to store somewhere the most important data.
	 * This is the place.
	 */
	class FScenePlayStorage {

		struct EntityStorage {
			TransformComponent transform;
			PointLightComponent light;
			ColorComponent color;
		};

	public:

		/// @brief default constructor
		FScenePlayStorage() = default;

		/**
		 * @brief Pushes entity to storage and saves it most important data
		 * @param entity entity, that user wants to be saved
		 */
		void pushEntityToStorage(const Entity& entity);

		/**
		 * @brief Loads back most important data for passed entity
		 * @param entity entity, that user wants to be loaded
		 */
		void loadEntityFromStorage(const Entity& entity);

		/// @brief Clear all storages
		void clear();

	private:

		void pushOperation(EntityStorage& storage, const Entity& entity);
		void loadOperation(const EntityStorage& storage, const Entity& entity);

		std::vector<EntityStorage> m_entityStorage;

	};


}

#endif // !MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H