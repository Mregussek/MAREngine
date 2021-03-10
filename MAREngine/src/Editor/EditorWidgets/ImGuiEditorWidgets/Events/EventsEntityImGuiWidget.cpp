/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "EventsEntityImGuiWidget.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../Impl/InspectorImGuiWidget.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Entity/EntityOperation.h"


namespace marengine {

    FSceneManagerEditor* FEventsEntityImGuiWidgets::s_pSceneManagerEditor{ nullptr };
    FInspectorImGuiWidget* FEventsEntityImGuiWidgets::s_pInspectorWidget{ nullptr };


    void FEventsEntityImGuiWidgets::create(FImGuiEditorServiceLocator* serviceLocator) {
        s_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        s_pInspectorWidget = serviceLocator->retrieve<FInspectorImGuiWidget>();
    }

	void FEventsEntityImGuiWidgets::onCreateEntity() {
		const Entity& createdEntity{ s_pSceneManagerEditor->getScene()->createEntity() };
		onSelectedEntity(createdEntity);
	}

	void FEventsEntityImGuiWidgets::onDestroyEntity(const Entity& entity) {
        s_pSceneManagerEditor->getScene()->destroyEntity(entity);
        s_pInspectorWidget->resetInspectedEntity();
        s_pSceneManagerEditor->initialize();
	}

	void FEventsEntityImGuiWidgets::onSelectedEntity(const Entity& entity) {
        s_pInspectorWidget->setEntityToInspect(entity);
	}

	void FEventsEntityImGuiWidgets::onUnselectedEntity(const Entity& entity) {

	}

	void FEventsEntityImGuiWidgets::onCopyEntity(const Entity& entity) {
		onCreateEntity();
		// onCreateEntity creates entity, then sets it to EntityWidgetPanel, so it can be got in that way
		const Entity& createdEntity{ s_pInspectorWidget->getInspectedEntity() };
		EntityOperation::copyEntity(entity, createdEntity);
        s_pSceneManagerEditor->initialize();
	}

	void FEventsEntityImGuiWidgets::onSetVisibleEntity(const Entity& entity) {

	}

	void FEventsEntityImGuiWidgets::onSetInvisibleEntity(const Entity& entity) {

	}

	void FEventsEntityImGuiWidgets::onAssignChild(const Entity& entity, const Entity& child) {
		entity.assignChild(child);
		onSelectedEntity(child);
	}

	void FEventsEntityImGuiWidgets::onRemoveChild(const Entity& entity, const Entity& child) {
		entity.removeChild(child);
		onUnselectedEntity(child);
	}

	void FEventsEntityImGuiWidgets::onCreateChild(const Entity& entity) {
		const Entity& createdChild{ s_pSceneManagerEditor->getScene()->createEntity() };
		onAssignChild(entity, createdChild);
	}

	void FEventsEntityImGuiWidgets::onDestroyChild(const Entity& entity, const Entity& child) {

	}


}
