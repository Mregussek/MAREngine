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


	void FScenePlayStorage::pushEntityToStorage(const Entity& entity) {
		typedef PlayModeStorageComponent::ComponentType ComType;

		auto& playModeComponent{ entity.addComponent<PlayModeStorageComponent>() };

		playModeComponent.components.insert({ ComType::TRANSFORM, entity.getComponent<TransformComponent>() });
		if (entity.hasComponent<ColorComponent>()) {
			playModeComponent.components.insert({ ComType::COLOR, entity.getComponent<ColorComponent>() });
		}
		if (entity.hasComponent<PointLightComponent>()) {
			playModeComponent.components.insert({ ComType::POINTLIGHT, entity.getComponent<PointLightComponent>() });
		}
	}

	template<PlayModeStorageComponent::ComponentType componentType, typename TComponent>
	static void loadComponent(const Entity& entity) {
		auto& playModeComponent{ entity.getComponent<PlayModeStorageComponent>() };

		auto search{ playModeComponent.components.find(componentType) };
		if (search != playModeComponent.components.cend()) {
			auto& component{ entity.getComponent<TComponent>() };
            component = std::get<TComponent>(search->second);
			playModeComponent.components.erase(search);
		}
	}

	void FScenePlayStorage::loadEntityFromStorage(const Entity& entity) {
		typedef PlayModeStorageComponent::ComponentType ComType;

		loadComponent<ComType::TRANSFORM, TransformComponent>(entity);
		loadComponent<ComType::COLOR, ColorComponent>(entity);
		loadComponent<ComType::POINTLIGHT, PointLightComponent>(entity);

		entity.removeComponent<PlayModeStorageComponent>();
	}


}
