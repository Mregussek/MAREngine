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

	void ScenePlayStorage::pushCollectionToStorage(const EntityCollection& collection) {
		CollectionStorage storage;

		for (const auto& entity : collection.getEntities()) {
			pushEntityToStorage(storage.entities, entity);
		}

		m_collectionStorage.push_back(storage);
	}

	void ScenePlayStorage::pushEntityToStorage(const Entity& entity) {
		EntityStorage storage;

		storage.transform = entity.getComponent<TransformComponent>();
		if (entity.hasComponent<LightComponent>()) {
			storage.light = entity.getComponent<LightComponent>();
		}
		if (entity.hasComponent<ColorComponent>()) {
			storage.color = entity.getComponent<ColorComponent>();
		}

		m_entityStorage.push_back(storage);
	}

	void ScenePlayStorage::pushEntityToStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity) {
		EntityStorage storage;

		storage.transform = entity.getComponent<TransformComponent>();
		if (entity.hasComponent<LightComponent>()) {
			storage.light = entity.getComponent<LightComponent>();
		}
		if (entity.hasComponent<ColorComponent>()) {
			storage.color = entity.getComponent<ColorComponent>();
		}

		vectorStorage.push_back(storage);
	}

	void ScenePlayStorage::loadCollectionFromStorage(const EntityCollection& collection) {
		auto& storage = m_collectionStorage.front();

		for (const auto& entity : collection.getEntities()) {
			loadEntityFromStorage(storage.entities, entity);
		}

		pop_front(m_collectionStorage);
	}

	void ScenePlayStorage::loadEntityFromStorage(const Entity& entity) {
		auto& storage = m_entityStorage.front();

		loadOperation(storage, entity);

		pop_front(m_entityStorage);
	}

	void ScenePlayStorage::loadEntityFromStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity) {
		const auto& storage = vectorStorage.front();

		loadOperation(storage, entity);

		pop_front(vectorStorage);
	}

	void ScenePlayStorage::loadOperation(const EntityStorage& storage, const Entity& entity) {
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
	}

	void ScenePlayStorage::clear() {
		m_entityStorage.clear();

		for (auto& collection : m_collectionStorage) { collection.clear(); }
	}


}
