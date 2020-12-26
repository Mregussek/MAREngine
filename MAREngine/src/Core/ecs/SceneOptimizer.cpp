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


#include "SceneOptimizer.h"
#include "Scene.h"
#include "ECSLogs.h"
#include "Entity/EntityCollection.h"
#include "Components/Components.h"


namespace marengine {

	
	int32_t SceneOptimizer::checkIfOBJhasBeenLoaded(const std::string& path, Scene* scene) {
		ECS_TRACE("SCENE_OPTIMIZER: checking if some other collection in scene contains given obj path...");

		const auto& collections{ scene->getCollections() };
		auto hasTheSameOBJ = [&path](const EntityCollection& collection) {
			if (!collection.hasComponent<CollectionRenderableComponent>()) {
				return false;
			}

			const auto& crc{ collection.getComponent<CollectionRenderableComponent>() };
			return crc.id == path;
		};

		const auto it = std::find_if(collections.cbegin(), collections.cend(), hasTheSameOBJ);
		if (it != collections.cend()) {
			ECS_INFO("SCENE_OPTIMIZER: found entity collection component in some collection with given path {}, returning pointer...", path);
			const auto collectionIndex{ (int32_t)std::distance(collections.cbegin(), it) };
			return collectionIndex;
		}
		
		ECS_WARN("SCENE_OPTIMIZER: cannot find entity collection component with given path {}...", path);
		return -1;
	}

	void SceneOptimizer::copyOBJtoOtherCollectionDuringSceneLoading(const EntityCollection& collection, const EntityCollection& collectionWithOBJ) {
		ECS_TRACE("SCENE_OPTIMIZER: copying OBJ to collection during scene file loading...");

		const auto& crcOBJ{ collectionWithOBJ.getComponent<CollectionRenderableComponent>() };
		const auto& entitiesOBJ{ collectionWithOBJ.getComponent<EntityCollectionComponent>().entities };
		const auto& crc = collection.addComponent<CollectionRenderableComponent>(crcOBJ);

		for (size_t i = 0; i < crc.entitiesRenderableCount; i++) {
			const auto& entity = collection.createEntity();
			const auto& renderableOBJ{ entitiesOBJ[i].getComponent<RenderableComponent>() };
			const auto& renderable = entity.addComponent<RenderableComponent>(renderableOBJ);
		}
	}

	void SceneOptimizer::copyOBJtoOtherCollectionOnSceneEvent(const EntityCollection& collection, const EntityCollection& collectionWithOBJ, const std::string& filename) {
		ECS_TRACE("SCENE_OPTIMIZER: copying OBJ to collection on scene event...");

		const auto& crcOBJ{ collectionWithOBJ.getComponent<CollectionRenderableComponent>() };
		const auto& entitiesOBJ{ collectionWithOBJ.getComponent<EntityCollectionComponent>().entities };
		const auto& crc = collection.addComponent<CollectionRenderableComponent>(crcOBJ);

		{ // modify collection's tag component
			auto& tag = collection.getComponent<TagComponent>();
			tag.tag = filename;
		}

		for (size_t i = 0; i < crc.entitiesRenderableCount; i++) {
			const auto& entity = collection.createEntity();
			const auto& renderableOBJ{ entitiesOBJ[i].getComponent<RenderableComponent>() };
			const auto& renderable = entity.addComponent<RenderableComponent>(renderableOBJ);

			auto& tag = entity.getComponent<TagComponent>();
			tag.tag = entitiesOBJ[i].getComponent<TagComponent>().tag;

			const auto& color = entity.addComponent<ColorComponent>();
		}
	}


}
