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

// Implementations...
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/DebugImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/EnvironmentPropertiesImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/InspectorImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/MainImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/MainMenuBarImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/SceneHierarchyImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/ScriptImGuiEditorWidgetImpl.h"
#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/Impl/ViewportImGuiEditorWidgetImpl.h"

#include "../../Editor/EditorWidgets/ImGuiEditorWidgets/ImGuiEditorWidgetsLocator.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


    void FImGuiEditorLayer::create(FSceneManagerEditor *pSceneManagerEditor, const RenderStatistics* pRenderStatistics) {
        m_pSceneManagerEditor = pSceneManagerEditor;

        m_serviceLocator.registerImGuiWidgets();

        auto* script = m_serviceLocator.retrieve<FScriptImGuiEditorWidgetImpl>();
        m_editorManager.pushPanel((IEditorWidget*)script);

        auto* viewport = m_serviceLocator.retrieve<FViewportImGuiEditorWidgetImpl>();
        viewport->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)viewport);

        auto* mainWidget = m_serviceLocator.retrieve<FMainImGuiEditorWidgetImpl>();
        mainWidget->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)mainWidget);

        auto* debug = m_serviceLocator.retrieve<FDebugImGuiEditorWidgetImpl>();
        debug->create(m_pSceneManagerEditor, pRenderStatistics);
        m_editorManager.pushPanel((IEditorWidget*)debug);

        auto* mainMenuBar = m_serviceLocator.retrieve<FMainMenuBarImGuiEditorWidgetImpl>();
        mainMenuBar->create();
        m_editorManager.pushPanel((IEditorWidget*)mainMenuBar);

        auto* sceneHierarchy = m_serviceLocator.retrieve<FSceneHierarchyImGuiEditorWidgetImpl>();
        sceneHierarchy->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)sceneHierarchy);

        auto* envProperties = m_serviceLocator.retrieve<FEnvironmentPropertiesImGuiEditorWidgetImpl>();
        envProperties->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)envProperties);

        auto* inspector = m_serviceLocator.retrieve<FInspectorImGuiEditorWidgetImpl>();
        inspector->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)inspector);

        m_editorManager.onCreate();
    }

    void FImGuiEditorLayer::update() {
        m_editorManager.update();
    }

    void FImGuiEditorLayer::close() {
        m_editorManager.destroy();
    }

    void FImGuiEditorLayer::renderToViewport() {
        m_serviceLocator.retrieve<FViewportImGuiEditorWidgetImpl>()->bind(m_pSceneManagerEditor->getScene()->getBackground());
    }


}
