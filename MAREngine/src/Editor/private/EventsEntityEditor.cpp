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


#include "../public/EventsEntityEditor.h"
#include "../public/ServiceLocatorEditor.h"
#include "../public/EditorWidget.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/Entity.h"


namespace marengine {

    FSceneManagerEditor* FEventsEntityEditor::s_pSceneManagerEditor{ nullptr };
    FInspectorEditorWidget* FEventsEntityEditor::s_pInspectorWidget{ nullptr };


    void FEventsEntityEditor::create(FServiceLocatorEditor* pServiceLocator) {
        s_pSceneManagerEditor = pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        s_pInspectorWidget = pServiceLocator->retrieve<FInspectorWidgetImGui>();
    }

	void FEventsEntityEditor::onCreateEntity() {
		const Entity& createdEntity{ s_pSceneManagerEditor->getScene()->createEntity() };
		onSelectedEntity(createdEntity);
	}

	void FEventsEntityEditor::onDestroyEntity(const Entity& entity) {
        s_pSceneManagerEditor->getScene()->destroyEntity(entity);
        s_pInspectorWidget->resetInspectedEntity();
        // TODO: optimize it, because we don't need to reinitialize the whole render pipeline during entity destruction
        s_pSceneManagerEditor->updateSceneAtBatchManager();
	}

	void FEventsEntityEditor::onSelectedEntity(const Entity& entity) {
        s_pInspectorWidget->setInspectedEntity(entity);
	}

	void FEventsEntityEditor::onUnselectedEntity(const Entity& entity) {

	}

	void FEventsEntityEditor::onCopyEntity(const Entity& entity) {
		//onCreateEntity();
		//// onCreateEntity creates entity, then sets it to EntityWidgetPanel, so it can be got in that way
		//const Entity& createdEntity{ s_pInspectorWidget->getInspectedEntity() };
		//EntityOperation::copyEntity(entity, createdEntity);
		//// TODO: during entity copy we don't need to reinitialize the whole scene
        //s_pSceneManagerEditor->pushSceneToPipeline();
	}

	void FEventsEntityEditor::onSetVisibleEntity(const Entity& entity) {

	}

	void FEventsEntityEditor::onSetInvisibleEntity(const Entity& entity) {

	}

	void FEventsEntityEditor::onAssignChild(const Entity& entity, const Entity& child) {
		entity.assignChild(child);
		onSelectedEntity(child);
	}

	void FEventsEntityEditor::onRemoveChild(const Entity& entity, const Entity& child) {
		entity.removeChild(child);
		onUnselectedEntity(child);
	}

	void FEventsEntityEditor::onCreateChild(const Entity& entity) {
		const Entity& createdChild{ s_pSceneManagerEditor->getScene()->createEntity() };
		onAssignChild(entity, createdChild);
	}

	void FEventsEntityEditor::onDestroyChild(const Entity& entity, const Entity& child) {

	}


}