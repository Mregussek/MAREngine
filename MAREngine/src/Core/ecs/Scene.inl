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


#ifndef MAR_ENGINE_SCENE_INL
#define MAR_ENGINE_SCENE_INL


#include "Scene.h"


namespace marengine {


	template<typename TComponent>
	MAR_NO_DISCARD auto Scene::getView() {
		return m_sceneRegistry.view<TComponent>();
	}

	template<typename TComponent>
	MAR_NO_DISCARD TComponent& Scene::getComponent(entt::entity entt_entity) {
		MAR_CORE_ASSERT(m_sceneRegistry.has<TComponent>(entt_entity), "SCENE: Passed entity {} does not have component {}", entt_entity, typeid(TComponent).name());
		return m_sceneRegistry.get<TComponent>(entt_entity);
	}


}


#endif // !MAR_ENGINE_SCENE_INL
