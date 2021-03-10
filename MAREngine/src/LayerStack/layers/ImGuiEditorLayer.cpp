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


#include "ImGuiEditorLayer.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Events/EventsEntityImGuiWidget.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


    void FImGuiEditorLayer::create(FSceneManagerEditor *pSceneManagerEditor, const RenderStatistics* pRenderStatistics) {
        m_pSceneManagerEditor = pSceneManagerEditor;
        m_serviceLocator.registerServices(pSceneManagerEditor, pRenderStatistics);

        // In what order should every window be rendered (sometimes it matters, last window will show up first)
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FScriptImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FViewportImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FMainImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FDebugImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FInspectorImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FMainMenuBarImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FSceneHierarchyImGuiWidget>());
        m_editorServiceManager.emplace((IEditorWidget*)m_serviceLocator.retrieve<FEnvironmentPropertiesImGuiWidget>());

        FEventsEntityImGuiWidgets::create(&m_serviceLocator);

        m_editorServiceManager.onCreate();
    }

    void FImGuiEditorLayer::update() {
        m_editorServiceManager.onUpdate();
    }

    void FImGuiEditorLayer::close() {
        m_editorServiceManager.onDestroy();
        m_serviceLocator.close();
    }

    void FImGuiEditorLayer::renderToViewport() {
        m_serviceLocator.retrieve<FViewportImGuiWidget>()->bind(m_pSceneManagerEditor->getScene()->getBackground());
    }


}
