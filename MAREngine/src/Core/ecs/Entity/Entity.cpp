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


	Entity::Entity(SceneRegistry* sceneRegistry) :
		m_sceneRegistry(sceneRegistry),
		m_entityHandle(sceneRegistry->m_registry.create())
	{
		ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
	}

	Entity::Entity(entt::entity entt_entity, SceneRegistry* sceneRegistry) :
		m_sceneRegistry(sceneRegistry),
		m_entityHandle(entt_entity)
	{
		ECS_TRACE("ENTITY: {} is constructed from entt::entity!", m_entityHandle);
	}

	const bool Entity::isValid() const {
		ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

		return m_sceneRegistry->m_registry.valid(m_entityHandle);
	}

	void Entity::fillEntityWithBasicComponents(const Entity& entity) {
		entity.addComponent<TagComponent>();
		entity.addComponent<TransformComponent>();
	}

	void Entity::destroyYourself() const {
		ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

		m_sceneRegistry->m_registry.destroy(m_entityHandle);

		ECS_INFO("ENTITY: destroyed yourself!");
	}

	const Entity& Entity::createChild() {
		const Entity& createdChild{ m_childs.emplace_back(m_sceneRegistry) };
		fillEntityWithBasicComponents(createdChild);
		return createdChild;
	}

	void Entity::destroyChild(size_t index) {
		destroyChild(m_childs[index]);
	}

	void Entity::destroyChild(const Entity& entity) {
		auto it = std::find_if(m_childs.begin(), m_childs.end(), [&entity](const Entity& iterator) {
			return 	&iterator == &entity;
		});

		const bool canDestroyChild{ it != m_childs.end() && (*it).isValid() };

		if (canDestroyChild) {
			(*it).destroyYourself();
			m_childs.erase(it);
		}
	}

	bool Entity::hasChilds() const {
		return !m_childs.empty();
	}

	const Entity& Entity::getChild(size_t index) const {
		return m_childs[index];
	}


}
