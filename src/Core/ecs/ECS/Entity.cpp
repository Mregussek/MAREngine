/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Entity.h"
#include "Components.h"


namespace mar {
	namespace ecs {


		Entity::Entity(Scene* scene)
			: m_scene(scene),
			m_entityHandle(scene->m_registry.create())
		{ 
			ECS_TRACE("ENTITY: {} is constructed!", m_entityHandle);
		}

		Entity::Entity(const Entity& other)
			: m_entityHandle(other.m_entityHandle),
			m_scene(other.m_scene)
		{
			ECS_TRACE("ENTITY: {} is copied!", m_entityHandle);
		}

		void Entity::addDefault() {
			auto& com = m_scene->m_registry.emplace<Components>(m_entityHandle);
			com.components.push_back(ECS_DEFAULT);

			ECS_TRACE("ENTITY: {} adding default component", m_entityHandle);
		}

		template<typename T>
		const bool Entity::hasComponent() const {
			ECS_TRACE("ENTITY: {} checking if has component on board!", m_entityHandle);

			return m_scene->m_registry.has<T>(m_entityHandle);
		}

		const bool Entity::isValid() const {
			ECS_TRACE("ENTITY: {} checking if is valid!", m_entityHandle);

			return m_scene->m_registry.valid(m_entityHandle);
		}

		Entity::operator bool() const {
			return isValid();
		}

		void Entity::addComponent(EntityComponents entcmp) {
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

		void Entity::destroyYourself() {
			ECS_TRACE("ENTITY: {} is going to destroy yourself!", m_entityHandle);

			m_scene->m_registry.destroy(m_entityHandle);

			ECS_INFO("ENTITY: destroyed yourself!");
		}


} }