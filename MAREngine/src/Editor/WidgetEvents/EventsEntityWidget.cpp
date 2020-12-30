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


#include "EventsEntityWidget.h"
#include "../WidgetPanels/EntityPanels/EntityWidgetPanel.h"
#include "../WidgetPanels/EntityPanels/EntityCollectionWidgetPanel.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityOperation.h"
#include "../../Core/ecs/SceneManager.h"
#include "../../Core/events/SceneEvents.h"


namespace marengine {


	void FEventsEntityWidget::onCreateEntity() {
		const Entity& createdEntity{ SceneManager::Instance->getScene()->createEntity() };
		onSelectedEntity(createdEntity);
	}

	void FEventsEntityWidget::onDestroyEntity(const Entity& entity) {
		SceneManager::Instance->getScene()->destroyEntity(entity);

		WEntityWidgetPanel::Instance->reset();
		SceneEvents::Instance().onEntityRemove();
	}

	void FEventsEntityWidget::onSelectedEntity(const Entity& entity) {
		WEntityCollectionWidgetPanel::Instance->reset();
		WEntityWidgetPanel::Instance->setCurrentEntity(entity);
	}

	void FEventsEntityWidget::onUnselectedEntity(const Entity& entity) {

	}

	void FEventsEntityWidget::onCopyEntity(const Entity& entity) {
		onCreateEntity();
		
		// onCreateEntity creates entity, then sets it to EntityWidgetPanel, so it can be got in that way
		const auto& createdEntity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
		EntityOperation::copyEntity(entity, createdEntity);

		SceneEvents::Instance().onEntityCopy(entity);
	}

	void FEventsEntityWidget::onSetVisibleEntity(const Entity& entity) {

	}

	void FEventsEntityWidget::onSetInvisibleEntity(const Entity& entity) {

	}


}