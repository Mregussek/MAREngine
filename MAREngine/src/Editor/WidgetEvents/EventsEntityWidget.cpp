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
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityOperation.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


	FEventsEntityWidget* FEventsEntityWidget::Instance{ nullptr };


	void FEventsEntityWidget::onCreateEntity() const {
		const Entity& createdEntity{ FSceneManagerEditor::Instance->getScene()->createEntity() };
		onSelectedEntity(createdEntity);
	}

	void FEventsEntityWidget::onDestroyEntity(const Entity& entity) const {
		FSceneManagerEditor::Instance->getScene()->destroyEntity(entity);

		WEntityWidgetPanel::Instance->reset();
		FSceneManagerEditor::Instance->initialize();
	}

	void FEventsEntityWidget::onSelectedEntity(const Entity& entity) const {
		WEntityWidgetPanel::Instance->setCurrentEntity(entity);
	}

	void FEventsEntityWidget::onUnselectedEntity(const Entity& entity) const {

	}

	void FEventsEntityWidget::onCopyEntity(const Entity& entity) const {
		onCreateEntity();
		
		// onCreateEntity creates entity, then sets it to EntityWidgetPanel, so it can be got in that way
		const auto& createdEntity{ WEntityWidgetPanel::Instance->getCurrentEntity() };
		EntityOperation::copyEntity(entity, createdEntity);
		FSceneManagerEditor::Instance->initialize();
	}

	void FEventsEntityWidget::onSetVisibleEntity(const Entity& entity) const {

	}

	void FEventsEntityWidget::onSetInvisibleEntity(const Entity& entity) const {

	}

	void FEventsEntityWidget::onAssignChild(const Entity& entity, const Entity& child) const {
		entity.assignChild(child);
		onSelectedEntity(child);
	}

	void FEventsEntityWidget::onRemoveChild(const Entity& entity, const Entity& child) const {
		entity.removeChild(child);
		onUnselectedEntity(child);
	}

	void FEventsEntityWidget::onCreateChild(const Entity& entity) const {
		const Entity& createdChild{ FSceneManagerEditor::Instance->getScene()->createEntity() };
		onAssignChild(entity, createdChild);
	}

	void FEventsEntityWidget::onDestroyChild(const Entity& entity, const Entity& child) const {

	}


}
