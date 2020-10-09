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


#include "ScenePlayStorage.h"
#include "Entity/Entity.h"
#include "Entity/EntityCollection.h"


namespace mar::ecs {


	template<typename T>
	void pop_front(std::vector<T>& vec) {
		MAR_CORE_ASSERT(!vec.empty(), "Cannot pop front, vec is empty!");
		vec.erase(vec.begin());
	}

	void ScenePlayStorage::pushCollectionToStorage(std::vector<CollectionStorage>& vector_storage, const EntityCollection& collection) {
		CollectionStorage storage;

		for (auto& entity : collection.getEntities())
			pushEntityToStorage(storage.entities, entity);

		vector_storage.push_back(storage);
	}

	void ScenePlayStorage::pushEntityToStorage(std::vector<EntityStorage>& vector_storage, const Entity& entity) {
		EntityStorage storage;

		storage.transform = entity.getComponent<TransformComponent>();
		if (entity.hasComponent<LightComponent>())
			storage.light = entity.getComponent<LightComponent>();
		if (entity.hasComponent<ColorComponent>())
			storage.color = entity.getComponent<ColorComponent>();

		vector_storage.push_back(storage);
	}

	void ScenePlayStorage::loadCollectionFromStorage(std::vector<CollectionStorage>& vector_storage, EntityCollection& collection) {
		auto& storage = vector_storage.front();

		for (size_t i = 0; i < collection.getEntities().size(); i++) {
			loadEntityFromStorage(storage.entities, collection.getEntity(i));
		}

		pop_front(vector_storage);
	}

	void ScenePlayStorage::loadEntityFromStorage(std::vector<EntityStorage>& vector_storage, Entity& entity) {
		auto& storage = vector_storage.front();

		auto& tran = entity.getComponent<TransformComponent>();

		tran.center = storage.transform.center;
		tran.angles = storage.transform.angles;
		tran.scale = storage.transform.scale;

		tran.recalculate();

		if (entity.hasComponent<LightComponent>()) {
			auto& light = entity.getComponent<LightComponent>();

			light.ambient = storage.light.ambient;
			light.diffuse = storage.light.diffuse;
			light.specular = storage.light.specular;
			light.quadratic = storage.light.quadratic;
			light.linear = storage.light.linear;
			light.shininess = storage.light.shininess;
			light.constant = storage.light.constant;
		}

		if (entity.hasComponent<ColorComponent>()) {
			auto& color = entity.getComponent<ColorComponent>();

			color.texture = storage.color.texture;
		}

		pop_front(vector_storage);
	}

	void ScenePlayStorage::clear() {
		entity_storage.clear();
		for (auto& collection : collection_storage)
			collection.clear();
	}


}
