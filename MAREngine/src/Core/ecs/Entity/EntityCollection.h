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


#ifndef MAR_ENGINE_ECS_ENTITY_COLLECTION_H
#define MAR_ENGINE_ECS_ENTITY_COLLECTION_H


#include "../../../mar.h"
#include "../ECSLogs.h"
#include "Entity.h"
#include "../SceneRegistry.h"
#include "../Components/Components.h"


namespace mar::ecs {


	struct EntityCollectionComponent {
		std::vector<Entity> entities;

		EntityCollectionComponent() = default;
		EntityCollectionComponent(const EntityCollectionComponent& ecc) = default;

		operator std::vector<Entity>& () { return entities; }
		operator const std::vector<Entity>& () const { return entities; }
	};

	class EntityCollection {

		friend class Scene;

	public:

		EntityCollection() = delete;
		EntityCollection(const EntityCollection& other) = default;

		EntityCollection(SceneRegistry* scene);

		void destroyYourself() const;

		const bool isValid() const;
		operator const bool() const;

		const Entity& createEntity() const;
		void destroyEntity(const Entity& entity) const;

		const std::vector<Entity>& getEntities() const;

		// ----------------------------------------------------
		// COLLECTION COMPONENT METHODS (definitions must be here, because of linker errors)
		// ----------------------------------------------------

		template<typename T>
		bool hasComponent() const {
			const bool hasComp{ m_scene->m_registry.has<T>(m_collectionHandle) };
			ECS_TRACE("ENTITY_COLLECTION: checking if {} collection contains component {}... result={}", m_collectionHandle, typeid(T).name(), hasComp);
			return hasComp;
		}

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) const {
			ECS_TRACE("ENTITY_COLLECTION: add component {} to collection {}", typeid(T).name(), m_collectionHandle);
			MAR_CORE_ASSERT(!hasComponent<T>(), "ENTITY_COLLECTION: {} does not have {} component!", m_collectionHandle, typeid(T).name());

			return m_scene->m_registry.emplace<T>(m_collectionHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() const {
			ECS_TRACE("ENTITY_COLLECTION: get component {} from collection {}", typeid(T).name(), m_collectionHandle);
			MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY_COLLECTION: {} does not have {} component!", m_collectionHandle, typeid(T).name());

			return m_scene->m_registry.get<T>(m_collectionHandle);
		}

		template<typename T>
		T& replaceComponent(const EntityCollection& other) const {
			ECS_TRACE("ENTITY_COLLECTION: replacing component {} given from collection {} to {}", typeid(T).name(), other.m_collectionHandle, m_collectionHandle);
			MAR_CORE_ASSERT(other.hasComponent<T>(), "ENTITY_COLLECTION: {} does not have {} component!", other.m_collectionHandle, typeid(T).name());

			return m_scene->m_registry.emplace_or_replace<T>(m_collectionHandle, other.getComponent<T>());
		}

		template<typename T>
		void removeComponent() const {
			ECS_TRACE("ENTITY_COLLECTION: removing {} from collection {}", typeid(T).name(), m_collectionHandle);
			MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY_COLLECTION: {} does not have {} component!", m_collectionHandle, typeid(T).name());

			m_scene->m_registry.remove<T>(m_collectionHandle);
		}

	private:

		entt::entity m_collectionHandle{ entt::null };
		SceneRegistry* m_scene{ nullptr };

	};


}


#endif // !MAR_ENGINE_ECS_ENTITY_COLLECTION_H
