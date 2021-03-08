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


#include "EventsEntityWidget.h"
#include "../Impl/InspectorImGuiWidget.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Entity/EntityOperation.h"


namespace marengine {


    void FEntityImGuiEditorEvents::create(FSceneManagerEditor* pSceneManagerEditor, FInspectorImGuiWidget* pInspectorWidget) {
        m_pSceneManagerEditor = pSceneManagerEditor;
        m_pInspectorWidget = pInspectorWidget;
    }

	void FEntityImGuiEditorEvents::onCreateEntity() const {
		const Entity& createdEntity{ m_pSceneManagerEditor->getScene()->createEntity() };
		onSelectedEntity(createdEntity);
	}

	void FEntityImGuiEditorEvents::onDestroyEntity(const Entity& entity) const {
		FSceneManagerEditor::Instance->getScene()->destroyEntity(entity);
        m_pInspectorWidget->resetInspectedEntity();
        m_pSceneManagerEditor->initialize();
	}

	void FEntityImGuiEditorEvents::onSelectedEntity(const Entity& entity) const {
		m_pInspectorWidget->setEntityToInspect(entity);
	}

	void FEntityImGuiEditorEvents::onUnselectedEntity(const Entity& entity) const {

	}

	void FEntityImGuiEditorEvents::onCopyEntity(const Entity& entity) const {
		onCreateEntity();
		// onCreateEntity creates entity, then sets it to EntityWidgetPanel, so it can be got in that way
		const Entity& createdEntity{ m_pInspectorWidget->getInspectedEntity() };
		EntityOperation::copyEntity(entity, createdEntity);
        m_pSceneManagerEditor->initialize();
	}

	void FEntityImGuiEditorEvents::onSetVisibleEntity(const Entity& entity) const {

	}

	void FEntityImGuiEditorEvents::onSetInvisibleEntity(const Entity& entity) const {

	}

	void FEntityImGuiEditorEvents::onAssignChild(const Entity& entity, const Entity& child) const {
		entity.assignChild(child);
		onSelectedEntity(child);
	}

	void FEntityImGuiEditorEvents::onRemoveChild(const Entity& entity, const Entity& child) const {
		entity.removeChild(child);
		onUnselectedEntity(child);
	}

	void FEntityImGuiEditorEvents::onCreateChild(const Entity& entity) const {
		const Entity& createdChild{ m_pSceneManagerEditor->getScene()->createEntity() };
		onAssignChild(entity, createdChild);
	}

	void FEntityImGuiEditorEvents::onDestroyChild(const Entity& entity, const Entity& child) const {

	}


}
