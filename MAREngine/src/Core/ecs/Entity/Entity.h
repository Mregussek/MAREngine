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


#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H


#include "../../../mar.h"
#include "../ECSLogs.h"
#include "../SceneRegistry.h"
#include "../Components/Components.h"


namespace mar::ecs {


	class Entity {
		friend class EntityCollection;
		friend class Scene;

	public:
		Entity() = delete;

		Entity(SceneRegistry* scene)
			: m_scene(scene),
			m_entityHandle(scene->m_registry.create())
		{
			ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
		}

		Entity(const Entity& other) :
			m_entityHandle(other.m_entityHandle),
			m_scene(other.m_scene)
		{
			ECS_TRACE("ENTITY: {} is copied!", m_entityHandle);
		}

		void addDefault() {
			auto& com = m_scene->m_registry.emplace<Components>(m_entityHandle);
			auto& rpc = m_scene->m_registry.emplace<RenderPipelineComponent>(m_entityHandle);

			ECS_TRACE("ENTITY: {} adding default component", m_entityHandle);
		}

		void copyDefault(Entity* other) {
			auto com = m_scene->m_registry.get<Components>(m_entityHandle);
			auto rpc = m_scene->m_registry.get<RenderPipelineComponent>(m_entityHandle);

			m_scene->m_registry.replace<Components>(other->m_entityHandle, com);
			m_scene->m_registry.replace<RenderPipelineComponent>(other->m_entityHandle, rpc);

			ECS_TRACE("ENTITY: copying default components from {} to {}", m_entityHandle, other->m_entityHandle);
		}

		const bool isValid() const {
			ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

			return m_scene->m_registry.valid(m_entityHandle);
		}

		operator const bool() const {
			return isValid();
		}

		void destroyYourself() {
			ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

			m_scene->m_registry.destroy(m_entityHandle);

			ECS_INFO("ENTITY: destroyed yourself!");
		}

		// ----------------------------------------------------
		// ENTITY COMPONENT METHODS (definitions must be here, because of linker errors)
		// ----------------------------------------------------

		template<typename T>
		const bool hasComponent() const {
			ECS_TRACE("ENTITY: {} - checking if entity {} has component!", typeid(T).name(), m_entityHandle);

			return m_scene->m_registry.has<T>(m_entityHandle);
		}

		template<typename T, typename... Args>
		T& addComponent(EntityComponents entcmp, Args&&... args) const {
			MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

			auto& com = getComponent<Components>();
			com.components.push_back(entcmp);

			ECS_TRACE("ENTITY: {} adding component explicitly - {}!", m_entityHandle, entcmp);

			return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() const {
			MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

			ECS_TRACE("ENTITY: {} - returning component from {}!", typeid(T).name(), m_entityHandle);

			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		void removeComponent(EntityComponents entcmp) const {
			MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");

			ECS_INFO("ENTITY: {} going to remove component {}", m_entityHandle, entcmp);

			auto& com = getComponent<Components>();
			auto it = std::find(com.components.begin(), com.components.end(), entcmp);
			if (it != com.components.end())
				com.components.erase(it);

			m_scene->m_registry.remove<T>(m_entityHandle);

			ECS_TRACE("ENTITY: {} removing component", m_entityHandle);
		}

	private:
		
		entt::entity m_entityHandle{ entt::null };
		SceneRegistry* m_scene{ nullptr };

	};


		


}


#endif // !MAR_ENGINE_ECS_ENTITY_H