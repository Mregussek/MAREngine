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


	template<typename T>
	MAR_NO_DISCARD const bool Entity::hasComponent() const {
		const bool hasComp{ m_sceneRegistry->m_registry.has<T>(m_entityHandle) };
		ECS_TRACE("ENTITY: checking if {} entity contains component {}... result={}", m_entityHandle, typeid(T).name(), hasComp);
		return hasComp;
	}

	template<typename T, typename... Args>
	T& Entity::addComponent(Args&&... args) const {
		ECS_TRACE("ENTITY: add component {} to entity {}", typeid(T).name(), m_entityHandle);
		MAR_CORE_ASSERT(!hasComponent<T>(), "ENTITY: entity {} already has {} component!", m_entityHandle, typeid(T).name());

		return m_sceneRegistry->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	MAR_NO_DISCARD T& Entity::getComponent() const {
		ECS_TRACE("ENTITY: get component {} from entity {}", typeid(T).name(), m_entityHandle);
		MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY: {} does not have {} component!", m_entityHandle, typeid(T).name());

		return m_sceneRegistry->m_registry.get<T>(m_entityHandle);
	}

	template<typename T>
	T& Entity::replaceComponent(const Entity& other) const {
		ECS_TRACE("ENTITY: replacing component {} given from entity {} to {}", typeid(T).name(), other.m_entityHandle, m_entityHandle);
		MAR_CORE_ASSERT(other.hasComponent<T>(), "ENTITY: {} does not have {} component!", other.m_entityHandle, typeid(T).name());

		return m_sceneRegistry->m_registry.replace<T>(m_entityHandle, other.getComponent<T>());
	}

	template<typename T>
	T& Entity::replaceComponent(const T& other) const {
		ECS_TRACE("ENTITY: replacing component {} in {} entity", typeid(T).name(), m_entityHandle);
		MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY: {} does not have {} component!", m_entityHandle, typeid(T).name());

		return m_sceneRegistry->m_registry.replace<T>(m_entityHandle, other);
	}

	template<typename T, typename... Args>
	MAR_NO_DISCARD T& Entity::get_addComponent(Args&&... args) const {
		ECS_TRACE("ENTITY: {} - get_addComponent at {}", typeid(T).name(), m_entityHandle);

		return m_sceneRegistry->m_registry.get_or_emplace<T>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	void Entity::removeComponent() const {
		ECS_TRACE("ENTITY: removing {} from entity {}", typeid(T).name(), m_entityHandle);
		MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY: {} does not have {} component!", m_entityHandle, typeid(T).name());

		m_sceneRegistry->m_registry.remove<T>(m_entityHandle);
	}


}



#endif // !MAR_ENGINE_ENTITY_INL

