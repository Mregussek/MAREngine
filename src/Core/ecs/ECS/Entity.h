/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H

#include "../../../mar.h"
#include "../ECSLogs.h"
#include "Components.h"
#include "../Scene.h"


namespace mar {
	namespace ecs {


		enum class EntityComponents;
		class Scene;


		class Entity {
			friend class Scene;

			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };

		public:
			Entity() = default;

			Entity(Scene* scene)
				: m_scene(scene),
				m_entityHandle(scene->m_registry.create())
			{ }

			Entity(const Entity& other) 
				: m_entityHandle(other.m_entityHandle),
				m_scene(other.m_scene)
			{}

			void addDefault() {
				auto& com = m_scene->m_registry.emplace<Components>(m_entityHandle);
				com.components.push_back(ECS_DEFAULT);

				ECS_TRACE("ENTITY: Adding default component");
			}

			template<typename T>
			const bool hasComponent() const {
				return m_scene->m_registry.has<T>(m_entityHandle);
			}

			template<typename T>
			T& getComponent() const {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				ECS_TRACE("ENTITY: returning component");

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			template<typename T, typename... Args>
			T& addComponent(EntityComponents entcmp, Args&&... args) {
				MAR_CORE_ASSERT(!hasComponent<T>(), "Entity already has this component!");

				auto& com = getComponent<Components>();
				com.components.push_back(entcmp);

				ECS_TRACE("ENTITY: adding component explicitly!");

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T, typename... Args>
			T& updateComponent(Args&&... args) {
				MAR_CORE_ASSERT(hasComponent<T>(), "Entity does not have this component!");

				ECS_TRACE("ENTITY: updating component");

				return m_scene->m_registry.patch<T>(m_entityHandle, std::forward<Args>(args)...);
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

			const bool isValid() const {
				return m_scene->m_registry.valid(m_entityHandle);
			}

			operator bool() const {
				return isValid();
			}

			void addComponent(EntityComponents entcmp) {
				ECS_TRACE("ENTITY: adding component implicitly!");

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

		private:
			void destroyYourself() {
				m_scene->m_registry.destroy(m_entityHandle);

				ECS_INFO("ENTITY: destroyed yourself!");
			}
		};


		


} }


#endif // !MAR_ENGINE_ECS_ENTITY_H