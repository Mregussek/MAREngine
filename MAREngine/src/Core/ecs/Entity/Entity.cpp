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


#include "Entity.h"


namespace marengine {


	Entity::Entity(SceneRegistry* scene) :
		m_scene(scene),
		m_entityHandle(scene->m_registry.create())
	{
		ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
	}

	Entity::Entity(entt::entity entity, SceneRegistry* scene) :
		m_scene(scene),
		m_entityHandle(entity)
	{
		ECS_TRACE("ENTITY: {} is constructed from entt::entity!", m_entityHandle);
	}

	void Entity::addDefault() const {
		m_scene->m_registry.emplace<RenderPipelineComponent>(m_entityHandle);

		ECS_TRACE("ENTITY: {} adding default component", m_entityHandle);
	}

	void Entity::copyDefault(const Entity& other) const {
		ECS_TRACE("ENTITY: copying default components from {} to {}", other.m_entityHandle, m_entityHandle);

		m_scene->m_registry.emplace<RenderPipelineComponent>(m_entityHandle, m_scene->m_registry.get<RenderPipelineComponent>(other.m_entityHandle));
	}

	const bool Entity::isValid() const {
		ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

		return m_scene->m_registry.valid(m_entityHandle);
	}

	void Entity::destroyYourself() const {
		ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

		m_scene->m_registry.destroy(m_entityHandle);

		ECS_INFO("ENTITY: destroyed yourself!");
	}


}
