/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "ECS/Entity.h"
#include "../../Editor/Filesystem/EditorFilesystem.h"



namespace mar {
	namespace ecs {


		Scene::Scene(std::string name)
			: m_name(std::move(name))
		{
			m_registry = entt::registry();

			ECS_INFO("SCENE: scene is created, with entt::registry! (called constructor)");
		}

		void Scene::shutdown() {
			m_registry.clear();
			
			ECS_INFO("SCENE: registry is cleared! (called destructor)");
		}

		// -------------------------------------------------------------
		// ENTITIES MANAGEMENT
		// -------------------------------------------------------------

		Entity& Scene::createEntity() {
			ECS_INFO("SCENE: going to create entity!");

			Entity entity{ this };

			entity.addDefault();
			entity.addComponent<TagComponent>(ECS_TAG);

			m_entities.push_back(entity);

			ECS_INFO("SCENE: created entity!");

			return m_entities[m_entities.size() - 1];
		}

		void Scene::destroyEntity(const int32_t& index) {
			ECS_INFO("SCENE: going to destroy entity!");

			if (m_entities[index].isValid()) {
				m_entities[index].destroyYourself();
				m_entities.erase(m_entities.begin() + index);

				ECS_INFO("SCENE: destroyed entity!");
				return;
			}
			else {
				ECS_INFO("SCENE: entity is not valid, so it cannot be destroyed!");
			}
		}


} }