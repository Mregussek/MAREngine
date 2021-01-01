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


namespace marengine {


	template<typename T>
	static void popFront(std::vector<T>& vec) {
		MAR_CORE_ASSERT(!vec.empty(), "Cannot pop front, vec is empty!");
		vec.erase(vec.begin());
	}

	void ScenePlayStorage::pushEntityToStorage(const Entity& entity) {
		auto& storage = m_entityStorage.emplace_back();
		pushOperation(storage, entity);
	}

	void ScenePlayStorage::pushEntityToStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity) {
		auto& storage = vectorStorage.emplace_back();
		pushOperation(storage, entity);
	}

	void ScenePlayStorage::pushOperation(EntityStorage& storage, const Entity& entity) {
		storage.transform = entity.getComponent<TransformComponent>();
		if (entity.hasComponent<LightComponent>()) {
			storage.light = entity.getComponent<LightComponent>();
		}
		if (entity.hasComponent<ColorComponent>()) {
			storage.color = entity.getComponent<ColorComponent>();
		}
	}

	void ScenePlayStorage::loadEntityFromStorage(const Entity& entity) {
		auto& storage = m_entityStorage.front();

		loadOperation(storage, entity);

		popFront(m_entityStorage);
	}

	void ScenePlayStorage::loadEntityFromStorage(std::vector<EntityStorage>& vectorStorage, const Entity& entity) {
		const auto& storage = vectorStorage.front();

		loadOperation(storage, entity);

		popFront(vectorStorage);
	}

	void ScenePlayStorage::loadOperation(const EntityStorage& storage, const Entity& entity) {
		auto& tran = entity.getComponent<TransformComponent>();

		tran.center = storage.transform.center;
		tran.angles = storage.transform.angles;
		tran.scale = storage.transform.scale;

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
	}


}
