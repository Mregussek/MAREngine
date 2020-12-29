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


#include "EntityCollectionWidgetEvents.h"
#include "../WidgetPanels/EntityPanels/EntityWidgetPanel.h"
#include "../WidgetPanels/EntityPanels/EntityCollectionWidgetPanel.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityOperation.h"
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/events/SceneEvents.h"


namespace marengine {


	void FEntityCollectionWidgetEvents::onCreateCollection() {
		const EntityCollection& createdCollection{ SceneManager::Instance->getScene()->createCollection() };
		onSelectedCollection(createdCollection);
	}

	void FEntityCollectionWidgetEvents::onDestroyCollection(const EntityCollection& collection) {
		SceneManager::Instance->getScene()->destroyCollection(collection);

		onUnselectedCollection();
		SceneEvents::Instance().onCollectionRemove();
	}

	void FEntityCollectionWidgetEvents::onEntityAddedToCollection(const EntityCollection& collection) {
		const Entity& createdEntity{ collection.createEntity() };
		WEntityWidgetPanel::Instance->setCurrentEntity(createdEntity);
	}

	void FEntityCollectionWidgetEvents::onEntityRemovedFromCollection(const EntityCollection& collection, const Entity& entity) {
		collection.destroyEntity(entity);
		onUnselectedEntityFromCollection();
		SceneEvents::Instance().onEntityRemove();
	}

	void FEntityCollectionWidgetEvents::onSelectedCollection(const EntityCollection& collection) {
		onUnselectedEntityFromCollection();
		WEntityCollectionWidgetPanel::Instance->setCurrentCollection(collection);
	}

	void FEntityCollectionWidgetEvents::onUnselectedCollection() {
		WEntityWidgetPanel::Instance->reset();
		WEntityCollectionWidgetPanel::Instance->reset();
	}

	void FEntityCollectionWidgetEvents::onSelectedEntityFromCollection(const Entity& entity) {
		WEntityWidgetPanel::Instance->setCurrentEntity(entity);
	}

	void FEntityCollectionWidgetEvents::onUnselectedEntityFromCollection() {
		WEntityWidgetPanel::Instance->reset();
	}

	void FEntityCollectionWidgetEvents::onCopyCollection(const EntityCollection& collection) {
		onCreateCollection();

		// onCreateEntityCollection creates collection, then sets it to EntityCollectionWidgetPanel, so it can be got in that way
		const auto& createdCollection{ WEntityCollectionWidgetPanel::Instance->getCurrentCollection() };
		EntityOperation::copyCollection(collection, createdCollection);
	}

	void FEntityCollectionWidgetEvents::onSetVisibleCollection(const EntityCollection& collection) {

	}

	void FEntityCollectionWidgetEvents::onSetInvisibleCollection(const EntityCollection& collection) {

	}

	template<typename T> static void FEntityCollectionWidgetEvents::onAddComponent(const EntityCollection& collection) { }
	template<typename T> static void FEntityCollectionWidgetEvents::onRemoveComponent(const EntityCollection& collection) { }
	template<typename T> static void FEntityCollectionWidgetEvents::onUpdateComponent(const EntityCollection& collection) { }


}
