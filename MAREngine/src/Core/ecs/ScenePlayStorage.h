/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H
#define MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H


#include "../../mar.h"
#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "ECS/EntityCollection.h"
#include "ECS/Systems.h"


namespace mar {
	namespace ecs {


		struct EntityStorage {
			TransformComponent transform;
			LightComponent light;
			ColorComponent color;
		};

		struct CollectionStorage {
			std::vector<EntityStorage> entities;
			
			void clear() { entities.clear(); }
		};

		struct ScenePlayStorage {
			std::vector<EntityStorage> entity_storage;
			std::vector<CollectionStorage> collection_storage;

			void pushCollectionToStorage(std::vector<CollectionStorage>& vector_storage, const EntityCollection& collection);
			void pushEntityToStorage(std::vector<EntityStorage>& vector_storage, const Entity& entity);

			void loadCollectionFromStorage(std::vector<CollectionStorage>& vector_storage, EntityCollection& collection);
			void loadEntityFromStorage(std::vector<EntityStorage>& vector_storage, Entity& entity);

			void clear();
		};


} }

#endif // !MAR_ENGINE_ECS_SCENE_PLAY_STORAGE_H