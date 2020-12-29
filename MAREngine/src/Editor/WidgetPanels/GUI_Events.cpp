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


#include "GUI_Events.h"
#include "EntityPanels/EntityWidgetPanel.h"
#include "EntityPanels/EntityCollectionWidgetPanel.h"
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/events/SceneEvents.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/EntityOperation.h"


namespace marengine {


	GUI_Events* GUI_Events::s_instance{ nullptr };

	const GUI_Events* GUI_Events::Instance() {
		return s_instance;
	}


	void GUI_Events::initialize() {
		s_instance = this;
	}

	void GUI_Events::onEntityCreated(SceneManager* sceneManager) const {
		WEntityCollectionWidgetPanel::Instance->reset();
		WEntityWidgetPanel::Instance->setCurrentEntity(sceneManager->getScene()->createEntity());
	}

	void GUI_Events::onEntityDeleted(SceneManager* sceneManager, const Entity& entity) const {
		sceneManager->getScene()->destroyEntity(entity);
		WEntityWidgetPanel::Instance->reset();
		SceneEvents::Instance().onEntityRemove();
	}

	void GUI_Events::onEntitySelected(const Entity& entity) const {
		WEntityCollectionWidgetPanel::Instance->reset();
		WEntityWidgetPanel::Instance->setCurrentEntity(entity);
	}

	void GUI_Events::onEntityCopied(SceneManager* sceneManager, const Entity& entityToCopy) const {
		const auto& entity = sceneManager->getScene()->createEntity();
		EntityOperation::copyEntity(entityToCopy, entity);

		onEntitySelected(entity);
		SceneEvents::Instance().onEntityCopy(entity);
	}

	void GUI_Events::onEntityCollectionCreated(SceneManager* sceneManager) const {
		WEntityWidgetPanel::Instance->reset();
		WEntityCollectionWidgetPanel::Instance->setCurrentCollection(sceneManager->getScene()->createCollection());
	}

	void GUI_Events::onEntityCollectionDeleted(SceneManager* sceneManager, const EntityCollection& collection) const {
		sceneManager->getScene()->destroyCollection(collection);
		WEntityCollectionWidgetPanel::Instance->reset();
		WEntityWidgetPanel::Instance->reset();
		SceneEvents::Instance().onCollectionRemove();
	}

	void GUI_Events::onEntityCollectionSelected(const EntityCollection& collection) const {
		WEntityWidgetPanel::Instance->reset();
		WEntityCollectionWidgetPanel::Instance->setCurrentCollection(collection);
	}

	void GUI_Events::onEntityCollectionCopied(SceneManager* sceneManager, const EntityCollection& collectionToCopy) const {
		const auto& collection = sceneManager->getScene()->createCollection();
		EntityOperation::copyCollection(collectionToCopy, collection);

		onEntityCollectionSelected(collection);
		SceneEvents::Instance().onCollectionCopy(collection);
	}

	void GUI_Events::onEntityCreatedAtCollection(const EntityCollection& collection) const {
		const auto& createdEntity = collection.createEntity();
		WEntityWidgetPanel::Instance->setCurrentEntity(createdEntity);
	}

	void GUI_Events::onEntityDeletedFromCollection(const EntityCollection& collection, const Entity& entity) const {
		collection.destroyEntity(entity);
		WEntityWidgetPanel::Instance->reset();
		SceneEvents::Instance().onEntityRemove();
	}


}
