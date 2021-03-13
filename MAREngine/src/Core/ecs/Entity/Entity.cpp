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


#include "Entity.h"


namespace marengine {


	Entity::Entity(entt::registry* pSceneRegistry) :
		m_pSceneRegistry(pSceneRegistry),
		m_entityHandle(m_pSceneRegistry->create())
	{
	}

	Entity::Entity(entt::entity enttEntity, entt::registry* pSceneRegistry) :
		m_pSceneRegistry(pSceneRegistry),
		m_entityHandle(enttEntity)
	{
	}

	const bool Entity::isValid() const {
		return m_pSceneRegistry->valid(m_entityHandle);
	}

	void Entity::fillEntityWithBasicComponents(const Entity& entity) {
		entity.addComponent<TagComponent>();
		entity.addComponent<TransformComponent>();
		entity.addComponent<ChildComponent>();
		entity.addComponent<LightBatchInfoComponent>();
		entity.addComponent<MeshBatchInfoComponent>();
	}

	void Entity::destroyYourself() const {
		m_pSceneRegistry->destroy(m_entityHandle);
	}

	const Entity& Entity::assignChild(const Entity& child) const {
		auto& childs{ getComponent<ChildComponent>().childs };
		return childs.emplace_back(child);
	}

	void Entity::removeChild(size_t index) const {
		removeChild(getChild(index));
	}

	void Entity::removeChild(const Entity& child) const {
		auto& childs{ getComponent<ChildComponent>().childs };

		auto it = std::find_if(childs.begin(), childs.end(), [&child](const Entity& iterator) {
			return 	&iterator == &child;
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


}
