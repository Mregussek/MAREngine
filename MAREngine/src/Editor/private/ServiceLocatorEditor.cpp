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


#include "../public/ServiceLocatorEditor.h"
#include "../../Core/ecs/SceneManagerEditor.h"


namespace marengine {

    template<typename THoldType>
    static void emplaceHolder(FServiceLocatorEditor* pServiceLocator, THoldType* pHoldType) {
        auto* pHolder = pServiceLocator->emplace<FHolderPtr<THoldType*>>();
        pHolder->pInstance = pHoldType;
    }


	void FServiceLocatorEditor::registerServices(FWindow* pWindow,
                                              FSceneManagerEditor* pSceneManagerEditor,
                                              FRenderStatistics* pRenderStatistics,
                                              FMeshManager* pMeshManager,
                                              FRenderManager* pRenderManager,
                                              FMaterialManager* pMaterialManager) {
		// Create registry and entity that will hold everything as components
        m_registry = entt::registry();
        m_entity = m_registry.create();

		// Create type holders, which must remember pointers
        emplaceHolder(this, pSceneManagerEditor);
        emplaceHolder(this, pRenderStatistics);
        emplaceHolder(this, pWindow);
        emplaceHolder(this, pMeshManager);
        emplaceHolder(this, pRenderManager);
        emplaceHolder(this, pMaterialManager);
	}

    template<> void FServiceLocatorEditor::create<EEditorContextType::IMGUI>() {
        // firstly we need to emplace all needed services
        auto* pScriptService{ emplace<FScriptWidgetImGui>() };
        auto* pContentService{ emplace<FContentBrowserImGui>() };
        auto* pViewportService{ emplace<FViewportWidgetImGui>() };
        auto* pMainService{ emplace<FMainWidgetImGui>() };
        auto* pDebugService{ emplace<FDebugWidgetImGui>() };
        auto* pMainMenuBarService{ emplace<FMainMenuBarWidgetImGui>() };
        auto* pSceneHierarchyService{ emplace<FSceneHierarchyWidgetImGui>() };
        auto* pEnvPropertiesService{ emplace<FEnvironmentPropertiesWidgetImGui>() };
        auto* pInspectorService{ emplace<FInspectorWidgetImGui>() };
        auto* pFilesystemService{ emplace<FFilesystemPopUpImGuiWidget>() };

        // then create, because they all need to call each other
        pScriptService->create(this);
        pContentService->create(this);
        pViewportService->create(this);
        pMainService->create(this);
        pDebugService->create(this);
        pMainMenuBarService->create(this);
        pSceneHierarchyService->create(this);
        pEnvPropertiesService->create(this);
        pInspectorService->create(this);
    }

	void FServiceLocatorEditor::close() {
        m_registry.destroy(m_entity);
        m_registry.clear();
	}


}
