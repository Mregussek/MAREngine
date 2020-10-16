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

		void destroyYourself();

		const bool isValid() const;
		operator const bool() const;

		const Entity& createEntity() const;
		void destroyEntity(int32_t entity_index) const;

		const std::vector<Entity>& getEntities() const;

		// ----------------------------------------------------
		// COLLECTION COMPONENT METHODS (definitions must be here, because of linker errors)
		// ----------------------------------------------------

		template<typename T>
		bool hasComponent() const {
			return m_scene->m_registry.has<T>(m_collectionHandle);
		}

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) const {
			MAR_CORE_ASSERT(!hasComponent<T>(), "ENTITY_COLLECTION: already has CollectionTagComponent!");

			return m_scene->m_registry.emplace<T>(m_collectionHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() const {
			MAR_CORE_ASSERT(hasComponent<T>(), "ENTITY_COLLECTION: does not have this component!");

			return m_scene->m_registry.get<T>(m_collectionHandle);
		}

		template<typename T>
		T& replaceComponent(const EntityCollection& other) const {
			MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");
			MAR_CORE_ASSERT(other.hasComponent<T>(), "Entity does not have this component!");

			ECS_TRACE("ENTITY_COLLECTION: {} - replacing component from {} to {}", typeid(T).name(), m_collectionHandle, other.m_collectionHandle);

			return m_scene->m_registry.replace<T>(other.m_collectionHandle, getComponent<T>());
		}

		template<typename T>
		void removeComponent() const {
			MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");

			ECS_INFO("ENTITY_COLLECTION: {} going to remove component {}", m_collectionHandle, typeid(T).name());

			m_scene->m_registry.remove<T>(m_collectionHandle);
		}

	private:

		entt::entity m_collectionHandle{ entt::null };
		SceneRegistry* m_scene{ nullptr };

	};


}


#endif // !MAR_ENGINE_ECS_ENTITY_COLLECTION_H
