/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H

#include "../../../mar.h"
#include "../ECSLogs.h"
#include "../Scene.h"
#include "Components.h"


namespace mar {
	namespace ecs {

		class Scene;


		class Entity {
			// ----------------------------------------------------
			// ENTITY PUBLIC METHODS
			// ----------------------------------------------------

		public:
			Entity() = default;
			Entity(Scene* scene);
			Entity(const Entity& other);

			const bool isValid() const;
			operator bool() const;

			void addDefault();

			void addComponent(EntityComponents entcmp);

			template<typename T>
			const bool hasComponent() const;

			void destroyYourself();

			// ----------------------------------------------------
			// ENTITY COMPONENT METHODS, THAT NEED TO BE HERE (cannot move them to Entity.cpp)
			// ----------------------------------------------------

			template<typename T, typename... Args>
			T& addComponent(EntityComponents entcmp, Args&&... args) {
				MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

				auto& com = getComponent<Components>();
				com.components.push_back(entcmp);

				ECS_TRACE("ENTITY: adding component explicitly!");

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			T& getComponent() const {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				ECS_TRACE("ENTITY: returning component");

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template<typename T>
			void removeComponent(EntityComponents entcmp) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				auto& com = getComponent<Components>();
				auto it = std::find(com.components.begin(), com.components.end(), entcmp);
				if (it != com.components.end())
					com.components.erase(it);

				m_scene->m_registry.remove<T>(m_entityHandle);

				ECS_TRACE("ENTITY: removing component");
			}

			// ----------------------------------------------------
			// ENTITY MEMBERS
			// ----------------------------------------------------

		private:
			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };
		};


		


} }


#endif // !MAR_ENGINE_ECS_ENTITY_H