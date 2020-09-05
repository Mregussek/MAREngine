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
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "ECS/EntityCollection.h"
#include "ECS/Systems.h"


namespace mar {
	namespace ecs {


		struct EntityStorage {
			TransformComponent transform;
			LightComponent light;
			ColorComponent color;
		};

		struct CollectionStorage {
			std::vector<EntityStorage> entities;
			
			void clear() { entities.clear(); }
		};

		struct ScenePlayStorage {
			std::vector<EntityStorage> entity_storage;
			std::vector<CollectionStorage> collection_storage;

			void pushCollectionToStorage(std::vector<CollectionStorage>& vector_storage, const EntityCollection& collection);
			void pushEntityToStorage(std::vector<EntityStorage>& vector_storage, const Entity& entity);

			void loadCollectionFromStorage(std::vector<CollectionStorage>& vector_storage, EntityCollection& collection);
			void loadEntityFromStorage(std::vector<EntityStorage>& vector_storage, Entity& entity);

			void clear();
		};


} }

#endif // !MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H