/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H

#include "../../../mar.h"
#include "../../../Debug/Log.h"
#include "../Component/Components.h"
#include "../Scene.h"


namespace mar {
	namespace ecs {


		enum class EntityComponents;


		class Entity {
			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };

		public:
			Entity() = delete;

			Entity(Scene* scene)
				: m_scene(scene),
				m_entityHandle(scene->m_registry.create())
			{ }

			Entity(const Entity& other) 
				: m_entityHandle(other.m_entityHandle),
				m_scene(other.m_scene)
			{}

			void addDefault() {
				m_scene->m_registry.emplace<Components>(m_entityHandle);
			}

			template<typename T>
			bool hasComponent() {
				return m_scene->m_registry.has<T>(m_entityHandle);
			}

			template<typename T>
			T& getComponent() {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template<typename T, typename... Args>
			T& addComponent(EntityComponents entcmp, Args&&... args) {
				MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

				auto& com = getComponent<Components>();
				com.components.push_back(entcmp);

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& updateComponent(Args&&... args) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				return m_scene->m_registry.patch<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			void removeComponent(EntityComponents entcmp) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				auto& com = getComponent<Components>();
				auto it = std::find(com.components.begin(), com.components.end(), entcmp);
				if (it != com.components.end()) 
					com.components.erase(it);

				m_scene->m_registry.remove<T>();
			}

			const bool isValid() const {
				return m_scene->m_registry.valid(m_entityHandle);
			}

			operator bool() const {
				return isValid();
			}
		};


} }


#endif // !MAR_ENGINE_ECS_ENTITY_H