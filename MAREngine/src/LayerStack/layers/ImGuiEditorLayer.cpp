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

        m_serviceLocator.registerImGuiWidgets();

        auto* filesystem = m_serviceLocator.retrieve<FFilesystemPopUpImGuiWidget>();

        auto* script = m_serviceLocator.retrieve<FScriptImGuiWidget>();
        m_editorManager.pushPanel((IEditorWidget*)script);

        auto* inspector = m_serviceLocator.retrieve<FInspectorImGuiWidget>();
        inspector->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)inspector);

        auto* viewport = m_serviceLocator.retrieve<FViewportImGuiWidget>();
        viewport->create(m_pSceneManagerEditor, inspector);
        m_editorManager.pushPanel((IEditorWidget*)viewport);

        auto* mainWidget = m_serviceLocator.retrieve<FMainImGuiWidget>();
        mainWidget->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)mainWidget);

        auto* debug = m_serviceLocator.retrieve<FDebugImGuiWidget>();
        debug->create(m_pSceneManagerEditor, pRenderStatistics);
        m_editorManager.pushPanel((IEditorWidget*)debug);

        auto* mainMenuBar = m_serviceLocator.retrieve<FMainMenuBarImGuiWidget>();
        mainMenuBar->create(filesystem);
        m_editorManager.pushPanel((IEditorWidget*)mainMenuBar);

        auto* sceneHierarchy = m_serviceLocator.retrieve<FSceneHierarchyImGuiWidget>();
        sceneHierarchy->create(m_pSceneManagerEditor, inspector);
        m_editorManager.pushPanel((IEditorWidget*)sceneHierarchy);

        auto* envProperties = m_serviceLocator.retrieve<FEnvironmentPropertiesImGuiWidget>();
        envProperties->create(m_pSceneManagerEditor);
        m_editorManager.pushPanel((IEditorWidget*)envProperties);

        FEventsEntityImGuiWidgets::create(m_pSceneManagerEditor, inspector);

        m_editorManager.onCreate();
    }

    void FImGuiEditorLayer::update() {
        m_editorManager.update();
    }

    void FImGuiEditorLayer::close() {
        m_editorManager.destroy();
    }

    void FImGuiEditorLayer::renderToViewport() {
        m_serviceLocator.retrieve<FViewportImGuiWidget>()->bind(m_pSceneManagerEditor->getScene()->getBackground());
    }


}
