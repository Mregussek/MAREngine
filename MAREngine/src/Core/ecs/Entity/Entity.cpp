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
		entity.addComponent<ChildComponent>();
		entity.addComponent<MeshBatchComponent>();
		entity.addComponent<LightBatchComponent>();
	}

	void Entity::destroyYourself() const {
		ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

		m_sceneRegistry->m_registry.destroy(m_entityHandle);

		ECS_INFO("ENTITY: destroyed yourself!");
	}

	void Entity::assignChild(const Entity& entity) const {
		auto& childs{ getComponent<ChildComponent>().childs };
		childs.push_back(entity);
	}

	void Entity::removeChild(size_t index) const {
		removeChild(getChild(index));
	}

	void Entity::removeChild(const Entity& entity) const {
		auto& childs{ getComponent<ChildComponent>().childs };

		auto it = std::find_if(childs.begin(), childs.end(), [&entity](const Entity& iterator) {
			return 	&iterator == &entity;
		});

		const bool canRemoveChild{ it != childs.end() };

		if (canRemoveChild) {
			childs.erase(it);
		}
	}

	bool Entity::hasChilds() const {
		return !getComponent<ChildComponent>().childs.empty();
	}

	const Entity& Entity::getChild(size_t index) const {
		return getComponent<ChildComponent>().childs[index];
	}

	const std::vector<Entity>& Entity::getChilds() const {
		return getComponent<ChildComponent>().childs;
	}

	bool Entity::hasAnyMaterial() const {
		return {
			hasComponent<ColorComponent>() || 
			hasComponent<Texture2DComponent>() || 
			hasComponent<TextureCubemapComponent>()
		};
	}


}
