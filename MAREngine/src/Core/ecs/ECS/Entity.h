/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H

#include "../../../mar.h"
#include "../ECSLogs.h"
#include "../Scene.h"
#include "ComponentsEntity.h"


namespace mar {
	namespace ecs {

		class Scene;


		class Entity {
			// ----------------------------------------------------
			// ENTITY PUBLIC METHODS
			// ----------------------------------------------------

		public:
			Entity() = delete;

			Entity(Scene* scene)
				: m_scene(scene),
				m_entityHandle(scene->m_registry.create())
			{
				ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
			}

			Entity(const Entity& other)
				: m_entityHandle(other.m_entityHandle),
				m_scene(other.m_scene)
			{
				ECS_TRACE("ENTITY: {} is copied!", m_entityHandle);
			}

			void addDefault() {
				auto& com = m_scene->m_registry.emplace<Components>(m_entityHandle);
				com.components.push_back(ECS_DEFAULT);

				ECS_TRACE("ENTITY: {} adding default component", m_entityHandle);
			}

			const bool isValid() const {
				ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

				return m_scene->m_registry.valid(m_entityHandle);
			}

			operator bool() const {
				return isValid();
			}

			void addComponent(EntityComponents entcmp) {
				ECS_TRACE("ENTITY: {} adding component implicitly!", m_entityHandle);

				switch (entcmp) {
				case ECS_RENDERABLE:
					addComponent<RenderableComponent>(ECS_RENDERABLE, "empty");
					break;
				case ECS_COLOR:
					addComponent<ColorComponent>(ECS_COLOR);
					break;
				case ECS_TEXTURE2D:
					addComponent<Texture2DComponent>(ECS_TEXTURE2D);
					break;
				case ECS_CUBEMAP:
					addComponent<TextureCubemapComponent>(ECS_CUBEMAP);
					break;
				case ECS_LIGHT:
					addComponent<LightComponent>(ECS_LIGHT);
					break;
				case ECS_CAMERA:
					addComponent<CameraComponent>(ECS_CAMERA);
					break;
				case ECS_SCRIPT:
					addComponent<ScriptComponent>(ECS_SCRIPT);
					break;
				case ECS_TRANSFORM:
					addComponent<TransformComponent>(ECS_TRANSFORM);
					break;
				case ECS_TAG:
					addComponent<TagComponent>(ECS_TAG);
					break;
				case ECS_DEFAULT:
					addComponent<ColorComponent>(ECS_DEFAULT);
					break;
				}
			}

			void destroyYourself() {
				ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

				m_scene->m_registry.destroy(m_entityHandle);

				ECS_INFO("ENTITY: destroyed yourself!");
			}

			// ----------------------------------------------------
			// ENTITY COMPONENT METHODS
			// ----------------------------------------------------

			template<typename T>
			const bool hasComponent() const {
				ECS_TRACE("ENTITY: {} checking if has component on board!", m_entityHandle);

				return m_scene->m_registry.has<T>(m_entityHandle);
			}

			template<typename T, typename... Args>
			T& addComponent(EntityComponents entcmp, Args&&... args) {
				MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

				auto& com = getComponent<Components>();
				com.components.push_back(entcmp);

				ECS_TRACE("ENTITY: {} adding component explicitly - {}!", m_entityHandle, entcmp);

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			T& getComponent() const {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				ECS_TRACE("ENTITY: {} returning component", m_entityHandle);

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template<typename T>
			void removeComponent(EntityComponents entcmp) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");

				ECS_INFO("ENTITY: {} going to remove component {}", m_entityHandle, entcmp);

				auto& com = getComponent<Components>();
				auto it = std::find(com.components.begin(), com.components.end(), entcmp);
				if (it != com.components.end())
					com.components.erase(it);

				m_scene->m_registry.remove<T>(m_entityHandle);

				ECS_TRACE("ENTITY: {} removing component", m_entityHandle);
			}

			// ----------------------------------------------------
			// ENTITY MEMBERS
			// ----------------------------------------------------

		private:
			friend class Scene;

			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };
		};


		


} }


#endif // !MAR_ENGINE_ECS_ENTITY_H