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


#include "ImGuiEditorServiceLocator.h"
#include "../../../Core/ecs/SceneManagerEditor.h"


namespace marengine {


	void FImGuiEditorServiceLocator::registerServices(FWindow* pWindow,
                                                      FSceneManagerEditor* pSceneManagerEditor,
                                                      FRenderStatistics* pRenderStatistics) {
		// Create registry and entity that will hold everything as components
	    m_imguiRegistry = entt::registry();
		m_imguiEntity = m_imguiRegistry.create();

		// Create type holders, which must remember pointers
        auto* sceneHolder = emplace<FImGuiTypeHolder<FSceneManagerEditor*>>();
        sceneHolder->pInstance = pSceneManagerEditor;

        auto* renderStatsHolder = emplace<FImGuiTypeHolder<FRenderStatistics*>>();
        renderStatsHolder->pInstance = pRenderStatistics;

        auto* windowHolder = emplace<FImGuiTypeHolder<FWindow*>>();
        windowHolder->pInstance = pWindow;

        // create instance of every widget
        emplace<FScriptWidgetImGui>();
        auto* viewport = emplace<FViewportWidgetImGui>();
        auto* mainWidget = emplace<FMainWidgetImGui>();
        auto* debug = emplace<FDebugWidgetImGui>();
        auto* mainMenuBar = emplace<FMainMenuBarWidgetImGui>();
        auto* sceneHierarchy = emplace<FSceneHierarchyWidgetImGui>();
        auto* envProperties = emplace<FEnvironmentPropertiesWidgetImGui>();
        auto* inspector = emplace<FInspectorWidgetImGui>();
		auto* filesystem = emplace<FFilesystemPopUpImGuiWidget>();

		// call create method, so that widgets are ready to use
        inspector->create(this);
        viewport->create(this);
        mainWidget->create(this);
        debug->create(this);
        mainMenuBar->create(this);
        sceneHierarchy->create(this);
        envProperties->create(this);
        filesystem->create(this);
	}

	void FImGuiEditorServiceLocator::close() {
        m_imguiRegistry.destroy(m_imguiEntity);
        m_imguiRegistry.clear();
	}


}
