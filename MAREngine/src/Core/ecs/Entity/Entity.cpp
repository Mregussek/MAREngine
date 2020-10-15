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


namespace mar::ecs {


	Entity::Entity(SceneRegistry* scene)
		: m_scene(scene),
		m_entityHandle(scene->m_registry.create())
	{
		ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
	}

	void Entity::addDefault() const {
		auto& com = m_scene->m_registry.emplace<Components>(m_entityHandle);
		auto& rpc = m_scene->m_registry.emplace<RenderPipelineComponent>(m_entityHandle);

		ECS_TRACE("ENTITY: {} adding default component", m_entityHandle);
	}

	void Entity::copyDefault(const Entity& other) const {
		auto com = m_scene->m_registry.get<Components>(m_entityHandle);
		auto rpc = m_scene->m_registry.get<RenderPipelineComponent>(m_entityHandle);

		m_scene->m_registry.replace<Components>(other.m_entityHandle, com);
		m_scene->m_registry.replace<RenderPipelineComponent>(other.m_entityHandle, rpc);

		ECS_TRACE("ENTITY: copying default components from {} to {}", m_entityHandle, other.m_entityHandle);
	}

	const bool Entity::isValid() const {
		ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

		return m_scene->m_registry.valid(m_entityHandle);
	}

	Entity::operator const bool() const {
		return isValid();
	}

	void Entity::destroyYourself() const {
		ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

		m_scene->m_registry.destroy(m_entityHandle);

		ECS_INFO("ENTITY: destroyed yourself!");
	}


}
