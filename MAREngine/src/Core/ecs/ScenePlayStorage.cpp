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


#include "ScenePlayStorage.h"
#include "Entity/Entity.h"


namespace marengine {


	template<typename T>
	static void popFront(std::vector<T>& vec) {
		MAR_CORE_ASSERT(!vec.empty(), "Cannot pop front, vec is empty!");
		vec.erase(vec.begin());
	}

	void FScenePlayStorage::pushEntityToStorage(const Entity& entity) {
		auto& storage = m_entityStorage.emplace_back();
		pushOperation(storage, entity);
	}

	void FScenePlayStorage::pushOperation(EntityStorage& storage, const Entity& entity) {
		storage.transform = entity.getComponent<TransformComponent>();
		if (entity.hasComponent<PointLightComponent>()) {
			storage.light = entity.getComponent<PointLightComponent>();
		}
		if (entity.hasComponent<ColorComponent>()) {
			storage.color = entity.getComponent<ColorComponent>();
		}
	}

	void FScenePlayStorage::loadEntityFromStorage(const Entity& entity) {
		auto& storage = m_entityStorage.front();

		loadOperation(storage, entity);

		popFront(m_entityStorage);
	}

	void FScenePlayStorage::loadOperation(const EntityStorage& storage, const Entity& entity) {
		auto& transformComponent{ entity.getComponent<TransformComponent>() };
		transformComponent = storage.transform;

		if (entity.hasComponent<PointLightComponent>()) {
			auto& pointLightComponent{ entity.getComponent<PointLightComponent>() };
			pointLightComponent = storage.light;
		}

		if (entity.hasComponent<ColorComponent>()) {
			auto& colorComponent = entity.getComponent<ColorComponent>();
			colorComponent = storage.color;
		}
	}

	void FScenePlayStorage::clear() {
		m_entityStorage.clear();
	}


}
