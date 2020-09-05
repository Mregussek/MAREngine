/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "ScenePlayStorage.h"


namespace mar {
	namespace ecs {


		template<typename T>
		void pop_front(std::vector<T>& vec) {
			MAR_CORE_ASSERT(!vec.empty(), "Cannot pop front, vec is empty!");
			vec.erase(vec.begin());
		}

		void ScenePlayStorage::pushCollectionToStorage(std::vector<CollectionStorage>& vector_storage, const EntityCollection& collection) {
			CollectionStorage storage;

			for (auto& entity : collection.getEntities())
				pushEntityToStorage(storage.entities, entity);

			vector_storage.push_back(storage);
		}

		void ScenePlayStorage::pushEntityToStorage(std::vector<EntityStorage>& vector_storage, const Entity& entity) {
			EntityStorage storage;

			storage.transform = entity.getComponent<TransformComponent>();
			if (entity.hasComponent<LightComponent>())
				storage.light = entity.getComponent<LightComponent>();
			if (entity.hasComponent<ColorComponent>())
				storage.color = entity.getComponent<ColorComponent>();

			vector_storage.push_back(storage);
		}

		void ScenePlayStorage::loadCollectionFromStorage(std::vector<CollectionStorage>& vector_storage, EntityCollection& collection) {
			auto& storage = vector_storage.front();

			for (size_t i = 0; i < collection.getEntities().size(); i++) {
				loadEntityFromStorage(storage.entities, collection.getEntity(i));
			}

			pop_front(vector_storage);
		}

		void ScenePlayStorage::loadEntityFromStorage(std::vector<EntityStorage>& vector_storage, Entity& entity) {
			auto& storage = vector_storage.front();

			auto& tran = entity.getComponent<TransformComponent>();

			tran.center = storage.transform.center;
			tran.angles = storage.transform.angles;
			tran.scale = storage.transform.scale;

			System::handleTransformComponent(tran);

			if (entity.hasComponent<LightComponent>()) {
				auto& light = entity.getComponent<LightComponent>();

				light.ambient = storage.light.ambient;
				light.diffuse = storage.light.diffuse;
				light.specular = storage.light.specular;
				light.quadratic = storage.light.quadratic;
				light.linear = storage.light.linear;
				light.shininess = storage.light.shininess;
				light.constant = storage.light.constant;
			}

			if (entity.hasComponent<ColorComponent>()) {
				auto& color = entity.getComponent<ColorComponent>();

				color.texture = storage.color.texture;
			}

			pop_front(vector_storage);
		}

		void ScenePlayStorage::clear() {
			entity_storage.clear();
			for (auto& collection : collection_storage)
				collection.clear();
		}

} }