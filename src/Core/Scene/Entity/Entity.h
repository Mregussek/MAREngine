/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H

#include "../../../mar.h"
#include "../../../Debug/Log.h"
#include "../Scene.h"


namespace mar {
	namespace ecs {

		enum class EntityComponents {
			COLOR,
			TEXTURE2D,
			CUBEMAP,
			RENDERABLE,
			TAG,
			TRANSFORM
		};

		class Entity {
			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };

		public:
			Entity() = default;

			Entity(entt::entity handle, Scene* scene)
				: m_entityHandle(handle),
				m_scene(scene)
			{ }

			Entity(const Entity& other) 
				: m_entityHandle(other.m_entityHandle),
				m_scene(other.m_scene)
			{}

			template<typename T>
			bool hasComponent() {
				return m_scene->m_registry.has<T>(m_entityHandle);
			}

			template<typename T, typename... Args>
			T& addComponent(Args&&... args) {
				MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			T& getComponent() {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template<typename T, typename... Args>
			T& updateComponent(Args&&... args) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				return m_scene->m_registry.patch<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			void removeComponent() {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				m_scene->m_registry.remove<T>();
			}

			const bool isValid() const {
				return m_scene->m_registry.valid(m_entityHandle);
			}

			operator bool() const {
				return isValid();
			}

			Entity copyEntity() {
				return { m_entityHandle, m_scene };
			}
		};


} }


#define ECS_RENDERABLE ::mar::ecs::EntityComponents::RENDERABLE
#define ECS_COLOR ::mar::ecs::EntityComponents::COLOR
#define ECS_TEXTURE2D ::mar::ecs::EntityComponents::TEXTURE2D
#define ECS_CUBEMAP ::mar::ecs::EntityComponents::CUBEMAP
#define ECS_TAG ::mar::ecs::EntityComponents::TAG
#define ECS_TRANSFORM ::mar::ecs::EntityComponents::TRANSFORM

#endif // !MAR_ENGINE_ECS_ENTITY_H