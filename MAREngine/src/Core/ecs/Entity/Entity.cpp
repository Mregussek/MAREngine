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

	bool Entity::isValid() const {
		return m_pSceneRegistry->valid(m_entityHandle);
	}

	void Entity::fillEntityWithBasicComponents(const Entity& entity) {
		entity.addComponent<CTag>();
		entity.addComponent<CTransform>();
		entity.addComponent<CChildren>();
	}

	void Entity::destroyYourself() const {
		m_pSceneRegistry->destroy(m_entityHandle);
	}

	const Entity& Entity::assignChild(const Entity& child) const {
		auto& children{ getComponent<CChildren>().children };
		return children.emplace_back(child);
	}

	void Entity::removeChild(size_t index) const {
		removeChild(getChild(index));
	}

	void Entity::removeChild(const Entity& child) const {
		auto& children{ getComponent<CChildren>().children };

		auto it = std::find_if(children.begin(), children.end(), [&child](const Entity& iterator) {
			return 	&iterator == &child;
		});

		const bool canRemoveChild{ it != children.end() };

		if (canRemoveChild) {
            children.erase(it);
		}
	}

	bool Entity::hasChildren() const {
		return !getComponent<CChildren>().children.empty();
	}

	const Entity& Entity::getChild(size_t index) const {
		return getComponent<CChildren>().children[index];
	}

	const std::vector<Entity>& Entity::getChildren() const {
		return getComponent<CChildren>().children;
	}


}
