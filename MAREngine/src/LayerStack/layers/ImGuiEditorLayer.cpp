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

        auto* script = m_serviceLocator.retrieve<FScriptImGuiWidget>();

        auto* inspector = m_serviceLocator.retrieve<FInspectorImGuiWidget>();
        inspector->create(&m_serviceLocator);

        auto* viewport = m_serviceLocator.retrieve<FViewportImGuiWidget>();
        viewport->create(&m_serviceLocator);

        auto* mainWidget = m_serviceLocator.retrieve<FMainImGuiWidget>();
        mainWidget->create(&m_serviceLocator);

        auto* debug = m_serviceLocator.retrieve<FDebugImGuiWidget>();
        debug->create(&m_serviceLocator);

        auto* mainMenuBar = m_serviceLocator.retrieve<FMainMenuBarImGuiWidget>();
        mainMenuBar->create(&m_serviceLocator);

        auto* sceneHierarchy = m_serviceLocator.retrieve<FSceneHierarchyImGuiWidget>();
        sceneHierarchy->create(&m_serviceLocator);

        auto* envProperties = m_serviceLocator.retrieve<FEnvironmentPropertiesImGuiWidget>();
        envProperties->create(&m_serviceLocator);

        // In what order should every window be rendered (sometimes it matters, last window will show up first)
        m_editorManager.pushPanel((IEditorWidget*)script);
        m_editorManager.pushPanel((IEditorWidget*)viewport);
        m_editorManager.pushPanel((IEditorWidget*)mainWidget);
        m_editorManager.pushPanel((IEditorWidget*)debug);
        m_editorManager.pushPanel((IEditorWidget*)inspector);
        m_editorManager.pushPanel((IEditorWidget*)mainMenuBar);
        m_editorManager.pushPanel((IEditorWidget*)sceneHierarchy);
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
