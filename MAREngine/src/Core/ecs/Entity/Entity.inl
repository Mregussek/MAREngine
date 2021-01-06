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


#ifndef MAR_ENGINE_ENTITY_INL
#define MAR_ENGINE_ENTITY_INL


#include "Entity.h"


namespace marengine {


	template<typename TComponent>
	MAR_NO_DISCARD const bool Entity::hasComponent() const {
		const bool hasComp{ m_sceneRegistry->has<TComponent>(m_entityHandle) };
		ECS_TRACE("ENTITY: checking if {} entity contains component {}... result={}", m_entityHandle, typeid(TComponent).name(), hasComp);

		return hasComp;
	}

	template<typename TComponent, typename... Args>
	TComponent& Entity::addComponent(Args&&... args) const {
		MAR_CORE_ASSERT(!hasComponent<TComponent>(), "ENTITY: entity {} already has {} component, cannot add it!", m_entityHandle, typeid(TComponent).name());

		return m_sceneRegistry->emplace<TComponent>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename TComponent>
	MAR_NO_DISCARD TComponent& Entity::getComponent() const {
		MAR_CORE_ASSERT(hasComponent<TComponent>(), "ENTITY: {} does not have {} component, cannot return it!", m_entityHandle, typeid(TComponent).name());

		return m_sceneRegistry->get<TComponent>(m_entityHandle);
	}

	template<typename TComponent>
	TComponent& Entity::replaceComponent(const Entity& other) const {
		MAR_CORE_ASSERT(other.hasComponent<TComponent>(), "ENTITY: other {} does not have {} component, cannot replace it!", other.m_entityHandle, typeid(TComponent).name());

		return m_sceneRegistry->replace<TComponent>(m_entityHandle, other.getComponent<TComponent>());
	}

	template<typename TComponent>
	TComponent& Entity::replaceComponent(const TComponent& other) const {
		MAR_CORE_ASSERT(hasComponent<TComponent>(), "ENTITY: {} does not have {} component, cannot replace it!", m_entityHandle, typeid(TComponent).name());

		return m_sceneRegistry->replace<TComponent>(m_entityHandle, other);
	}

	template<typename TComponent, typename... Args>
	MAR_NO_DISCARD TComponent& Entity::get_addComponent(Args&&... args) const {
		ECS_TRACE("ENTITY: {} - get_addComponent at {}", typeid(TComponent).name(), m_entityHandle);

		return m_sceneRegistry->get_or_emplace<TComponent>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename TComponent>
	void Entity::removeComponent() const {
		MAR_CORE_ASSERT(hasComponent<TComponent>(), "ENTITY: {} does not have {} component, cannot remove it!", m_entityHandle, typeid(TComponent).name());

		m_sceneRegistry->remove<TComponent>(m_entityHandle);
	}


}



#endif // !MAR_ENGINE_ENTITY_INL

