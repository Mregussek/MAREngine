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


#include "EntityCollection.h"


namespace mar::ecs {


	EntityCollection::EntityCollection(SceneRegistry* scene) :
		m_scene(scene),
		m_collectionHandle(scene->m_registry.create())
	{}

	const bool EntityCollection::isValid() const {
		ECS_TRACE("ENTITY: {} checking if is valid!", m_collectionHandle);

		return m_scene->m_registry.valid(m_collectionHandle);
	}

	EntityCollection::operator const bool() const {
		return isValid();
	}

	void EntityCollection::destroyYourself() const {
		auto& entitiesVector = getComponent<EntityCollectionComponent>().entities;
		for (size_t i = 0; i < entitiesVector.size(); i++) {
			if (entitiesVector[i].isValid()) {
				entitiesVector[i].destroyYourself();
				entitiesVector.erase(entitiesVector.begin() + i);

				ECS_TRACE("ENTITY_COLLECTION: from collection entity at {} is deleted!", i);
			}
			else {
				ECS_TRACE("ENTITY_COLLECTION: from collection entity at {} is not valid, so it is not deleted!!", i);
			}
		}

		m_scene->m_registry.destroy(m_collectionHandle);
	}

	const Entity& EntityCollection::createEntity() const {
		ECS_INFO("ENTITY_COLLECTION: going to create entity!");

		auto& entitiesVector = getComponent<EntityCollectionComponent>().entities;

		const auto& entity = entitiesVector.emplace_back(m_scene);

		entity.addDefault();
		entity.addComponent<TagComponent>();
		entity.addComponent<TransformComponent>();

		ECS_INFO("EntityCollection: created entity {} at collection {}!", entity.m_entityHandle, m_collectionHandle);

		return entity;
	}

	void EntityCollection::destroyEntity(const Entity& entity) const {
		auto& entitiesVector = getComponent<EntityCollectionComponent>().entities;

		auto it = std::find_if(entitiesVector.begin(), entitiesVector.end(), [&entity](const Entity& iterator) {
			return 	&iterator == &entity;
		});

		if (it != entitiesVector.end() && (*it).isValid()) {
			(*it).destroyYourself();
			entitiesVector.erase(it);
		}
	}

	const std::vector<Entity>& EntityCollection::getEntities() const {
		return getComponent<EntityCollectionComponent>().entities;
	}


}
