/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "Scene.h"
#include "ECS/ComponentsEntity.h"
#include "ECS/ComponentsCollection.h"
#include "ECS/Entity.h"
#include "ECS/EntityCollection.h"


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

		Scene* Scene::createEmptyScene(std::string name) {
			Scene* scene = new Scene(std::move(name));

			{
				auto& entity = scene->createEntity();
				entity.addComponent<CameraComponent>(ECS_CAMERA);

				auto& cam = entity.getComponent<TagComponent>();
				cam.tag = "Camera";
			}
			{
				auto& entity = scene->createEntity();
				entity.addComponent<LightComponent>(ECS_LIGHT);

				auto& light = entity.getComponent<TagComponent>();
				light.tag = "Light";
			}

			return scene;
		}

		// -------------------------------------------------------------
		// ENTITIES MANAGEMENT
		// -------------------------------------------------------------

		Entity& Scene::createEntity() {
			ECS_INFO("SCENE: going to create entity!");

			Entity entity{ this };

			entity.addDefault();
			entity.addComponent<TagComponent>(ECS_TAG);
			entity.addComponent<TransformComponent>(ECS_TRANSFORM);

			m_entities.push_back(entity);

			ECS_INFO("SCENE: created entity!");

			return m_entities[m_entities.size() - 1];
		}

		void Scene::destroyEntity(int32_t index) {
			ECS_INFO("SCENE: going to destroy entity at {}!", index);

			if (m_entities[index].isValid()) {
				m_entities[index].destroyYourself();
				m_entities.erase(m_entities.begin() + index);

				ECS_INFO("SCENE: destroyed entity at {}!", index);
				return;
			}
			else {
				ECS_INFO("SCENE: entity at {} is not valid, so it cannot be destroyed!", index);
			}
		}

		const std::vector<Entity>& Scene::getEntities() const { return m_entities; }
		Entity& Scene::getEntity(size_t index) { return m_entities[index]; }

		// -------------------------------------------------------------
		// ENTITIES COLLECTIONS MANAGEMENT
		// -------------------------------------------------------------

		EntityCollection& Scene::createCollection() {
			ECS_INFO("SCENE: going to create entity collection!");

			EntityCollection collection{ this };

			collection.addComponent<CollectionTagComponent>("DefaultName");

			m_collections.push_back(collection);

			return m_collections[m_collections.size() - 1];
		}

		void Scene::destroyCollection(int32_t index) {
			ECS_INFO("SCENE: going to destroy collection at {}", index);

			m_collections[index].destroyYourself();
			m_collections.erase(m_collections.begin() + index);

			ECS_INFO("SCENE: collection at {} is deleted properly!", index);
		}

		void Scene::destroyEntityAtCollection(int32_t collection_index, int32_t entity_index) {
			m_collections[collection_index].destroyEntity(entity_index);

			ECS_INFO("SCENE: called destroyEntity({}) at collection {}", entity_index, collection_index);
		}

		const std::vector<EntityCollection>& Scene::getCollections() const { return m_collections; }
		EntityCollection& Scene::getCollection(size_t index) { return m_collections[index]; }


} }