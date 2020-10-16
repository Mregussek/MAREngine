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


#ifndef MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H
#define MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H


#include "../../mar.h"
#include "Components/Components.h"


namespace mar::ecs {

	class Entity; class EntityCollection;


	struct EntityStorage {

		TransformComponent transform;
		LightComponent light;
		ColorComponent color;

	};

	struct CollectionStorage {

		std::vector<EntityStorage> entities;
		
		void clear() { 
			entities.clear(); 
		}
	};

	class ScenePlayStorage {
	public:

		ScenePlayStorage() = default;

		void pushEntityToStorage(const Entity& entity);
		void pushCollectionToStorage(const EntityCollection& collection);

		void loadEntityFromStorage(const Entity& entity);
		void loadCollectionFromStorage(const EntityCollection& collection);

		void clear();

	private:

		void pushEntityToStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity);
		void pushOperation(EntityStorage& storage, const Entity& entity);

		void loadEntityFromStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity);
		void loadOperation(const EntityStorage& storage, const Entity& entity);

		std::vector<EntityStorage> m_entityStorage;
		std::vector<CollectionStorage> m_collectionStorage;

	};


}

#endif // !MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H