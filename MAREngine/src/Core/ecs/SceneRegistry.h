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


#ifndef MAR_ENGINE_ECS_SCENE_REGISTRY_H
#define MAR_ENGINE_ECS_SCENE_REGISTRY_H


#include "../../mar.h"
#include "ECSLogs.h"


namespace mar::ecs {

	class Entity;
	class EntityCollection;
	

	class SceneRegistry {
		friend class Entity;
		friend class EntityCollection;

		entt::registry m_registry;

	public:
		SceneRegistry() {
			m_registry = entt::registry();
			
			ECS_TRACE("SCENE_REGISTRY: created!");
		}

		void cleanup() {
			m_registry.clear();

			ECS_TRACE("SCENE_REGISTRY: cleanup!");
		}
	};


}

#endif // !MAR_ENGINE_ECS_SCENE_REGISTRY_H
