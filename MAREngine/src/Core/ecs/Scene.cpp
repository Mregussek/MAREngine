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


#include "Scene.h"
#include "Components/Components.h"
#include "Entity/Entity.h"
#include "Entity/EntityCollection.h"
#include "ECSLogs.h"


namespace marengine {


	Scene::Scene(std::string name)
		: m_name(std::move(name))
	{
		m_sceneRegistry = SceneRegistry();

		ECS_INFO("SCENE: scene is created, with entt::registry! (called constructor)");
	}

	void Scene::shutdown() {
		std::for_each(m_container.m_entities.begin(), m_container.m_entities.end(), [this](const Entity& entity) {
			destroyEntity(entity);
		});

		std::for_each(m_container.m_collections.begin(), m_container.m_collections.end(), [this](const EntityCollection& collection) {
			destroyCollection(collection);
		});

		m_sceneRegistry.cleanup();

		ECS_INFO("SCENE: registry is cleared! (called destructor)");
	}

	Scene* Scene::createEmptyScene(std::string name) {
		Scene* scene = new Scene(std::move(name));

		{ // create Camera entity
			const Entity& cameraEntity{ scene->createEntity() };
			cameraEntity.addComponent<CameraComponent>();

			TagComponent& tag = cameraEntity.getComponent<TagComponent>();
			tag.tag = "CameraEntity";
		}
		{ // create Light Entity
			const Entity& lightEntity{ scene->createEntity() };
			lightEntity.addComponent<LightComponent>();

			TagComponent& tag = lightEntity.getComponent<TagComponent>();
			tag.tag = "LightEntity";
		}

		return scene;
	}

	// -------------------------------------------------------------
	// ENTITIES MANAGEMENT
	// -------------------------------------------------------------

	const Entity& Scene::createEntity() {
		ECS_INFO("SCENE: going to create entity!");

		const Entity& entity{ m_container.m_entities.emplace_back(&m_sceneRegistry) };
		Entity::fillEntityWithBasicComponents(entity);

		ECS_INFO("SCENE: created entity {} at sceme {}!", entity.m_entityHandle, m_name);

		return entity;
	}

	void Scene::destroyEntity(const Entity& entity) {
		ECS_INFO("SCENE: going to destroy entity at {}!", entity.m_entityHandle);

		auto it = std::find_if(m_container.m_entities.begin(), m_container.m_entities.end(), [&entity](const Entity& iterator) {
			return 	&iterator == &entity;
		});

		const bool canDestroyEntity{ it != m_container.m_entities.end() && (*it).isValid() };

		if (canDestroyEntity) {
			(*it).destroyYourself();
			m_container.m_entities.erase(it);
		}
	}

	const std::vector<Entity>& Scene::getEntities() const { 
		return m_container.m_entities;
	}

	// -------------------------------------------------------------
	// ENTITIES COLLECTIONS MANAGEMENT
	// -------------------------------------------------------------

	const EntityCollection& Scene::createCollection() {
		ECS_INFO("SCENE: going to create entity collection!");

		const EntityCollection& collection{ m_container.m_collections.emplace_back(&m_sceneRegistry) };

		collection.addComponent<EntityCollectionComponent>();
		collection.addComponent<TagComponent>("DefaultName");
		collection.addComponent<TransformComponent>();

		ECS_TRACE("SCENE: create collection {} at scene {}", collection.m_collectionHandle, m_name);

		return collection;
	}

	void Scene::destroyCollection(const EntityCollection& collection) {
		ECS_INFO("SCENE: going to destroy collection at {}!", collection.m_collectionHandle);

		auto it = std::find_if(m_container.m_collections.begin(), m_container.m_collections.end(), [&collection](const EntityCollection& iterator) {
			return 	&iterator == &collection;
		});

		const bool canDestroyCollection{ it != m_container.m_collections.end() && (*it).isValid() };

		if (canDestroyCollection) {
			(*it).destroyYourself();
			m_container.m_collections.erase(it);
		}
	}

	const std::vector<EntityCollection>& Scene::getCollections() const { 
		return m_container.m_collections;
	}


}