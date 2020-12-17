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
#include "EntityPanels/GUI_EntityPanel.h"
#include "EntityPanels/GUI_EntityCollectionPanel.h"
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/ecs/SceneEvents.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/EntityOperation.h"


namespace mar::editor {


	GUI_Events* GUI_Events::s_instance{ nullptr };

	const GUI_Events* GUI_Events::Instance() {
		return s_instance;
	}


	void GUI_Events::initialize() {
		s_instance = this;
	}

	void GUI_Events::onEntityCreated(ecs::SceneManager* sceneManager) const {
		GUI_EntityCollectionPanel::Instance()->reset();
		GUI_EntityPanel::Instance()->setCurrentEntity(sceneManager->getScene()->createEntity());
	}

	void GUI_Events::onEntityDeleted(ecs::SceneManager* sceneManager, const ecs::Entity& entity) const {
		sceneManager->getScene()->destroyEntity(entity);
		GUI_EntityPanel::Instance()->reset();
		ecs::SceneEvents::Instance().onEntityRemove();
	}

	void GUI_Events::onEntitySelected(const ecs::Entity& entity) const {
		GUI_EntityCollectionPanel::Instance()->reset();
		GUI_EntityPanel::Instance()->setCurrentEntity(entity);
	}

	void GUI_Events::onEntityCopied(ecs::SceneManager* sceneManager, const ecs::Entity& entityToCopy) const {
		const auto& entity = sceneManager->getScene()->createEntity();
		ecs::EntityOperation::copyEntity(entityToCopy, entity);

		onEntitySelected(entity);
		ecs::SceneEvents::Instance().onEntityCopy(entity);
	}

	void GUI_Events::onEntityCollectionCreated(ecs::SceneManager* sceneManager) const {
		GUI_EntityPanel::Instance()->reset();
		GUI_EntityCollectionPanel::Instance()->setCurrentCollection(sceneManager->getScene()->createCollection());
	}

	void GUI_Events::onEntityCollectionDeleted(ecs::SceneManager* sceneManager, const ecs::EntityCollection& collection) const {
		sceneManager->getScene()->destroyCollection(collection);
		GUI_EntityCollectionPanel::Instance()->reset();
		GUI_EntityPanel::Instance()->reset();
		ecs::SceneEvents::Instance().onCollectionRemove();
	}

	void GUI_Events::onEntityCollectionSelected(const ecs::EntityCollection& collection) const {
		GUI_EntityPanel::Instance()->reset();
		GUI_EntityCollectionPanel::Instance()->setCurrentCollection(collection);
	}

	void GUI_Events::onEntityCollectionCopied(ecs::SceneManager* sceneManager, const ecs::EntityCollection& collectionToCopy) const {
		const auto& collection = sceneManager->getScene()->createCollection();
		ecs::EntityOperation::copyCollection(collectionToCopy, collection);

		onEntityCollectionSelected(collection);
		ecs::SceneEvents::Instance().onCollectionCopy(collection);
	}

	void GUI_Events::onEntityCreatedAtCollection(const ecs::EntityCollection& collection) const {
		const auto& createdEntity = collection.createEntity();
		GUI_EntityPanel::Instance()->setCurrentEntity(createdEntity);
	}

	void GUI_Events::onEntityDeletedFromCollection(const ecs::EntityCollection& collection, const ecs::Entity& entity) const {
		collection.destroyEntity(entity);
		GUI_EntityPanel::Instance()->reset();
		ecs::SceneEvents::Instance().onEntityRemove();
	}


}
