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


#include "EditorLayer.h"
#include "../../Editor/public/EventsEntityEditor.h"
#include "../../Core/ecs/SceneManagerEditor.h"
#include "../../Core/ecs/Scene.h"


namespace marengine {


    void FEditorLayerImGui::create(FWindow* pWindow,
                                   FSceneManagerEditor *pSceneManagerEditor,
                                   FMeshManager* pMeshManager,
                                   FRenderManager* pRenderManager,
                                   FMaterialManager* pMaterialManager,
                                   FRenderStatistics* pRenderStatistics) {

        m_serviceLocator.registerServices(pWindow,
                                          pSceneManagerEditor,
                                          pRenderStatistics,
                                          pMeshManager,
                                          pRenderManager,
                                          pMaterialManager);

        m_serviceLocator.create<EEditorContextType::IMGUI>();

        // In what order should every window be rendered
        // (sometimes it matters, last window will show up first)
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FScriptWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FViewportWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FMainWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FDebugWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FInspectorWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FMainMenuBarWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FSceneHierarchyWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FEnvironmentPropertiesWidgetImGui>());
        m_serviceManager.emplace(
                (FEditorWidget*)m_serviceLocator.retrieve<FContentBrowserImGui>());

        FEntityEventsEditor::create(&m_serviceLocator);

        m_serviceManager.onCreate();
    }

    void FEditorLayerImGui::begin() {
        
    }

    void FEditorLayerImGui::update() {
        m_serviceManager.onUpdate();
    }

    void FEditorLayerImGui::end() {

    }

    void FEditorLayerImGui::close() {
        m_serviceManager.onDestroy();
        m_serviceLocator.close();
    }


}
