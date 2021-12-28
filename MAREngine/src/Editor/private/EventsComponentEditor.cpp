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


#include "../public/EventsComponentEditor.h"
#include "../public/ServiceLocatorEditor.h"
#include "../../Core/ecs/Entity/EventsComponentEntity.h" // component add/update/remove events
#include "../../Core/filesystem/public/FileManager.h"
#include "../../Core/graphics/public/MaterialManager.h"
#include "../../ProjectManager.h"
#include "../../Logging/Logger.h"


namespace marengine {

    FScriptEditorWidget* FEventsComponentEditor::s_pScriptWidget{ nullptr };
    FMaterialManager* FEventsComponentEditor::s_pMaterialManager{ nullptr };


    void FEventsComponentEditor::create(FServiceLocatorEditor* pServiceLocator) {
        s_pScriptWidget = pServiceLocator->retrieve<FScriptWidgetImGui>();
        s_pMaterialManager =
                pServiceLocator->retrieve<FHolderPtr<FMaterialManager*>>()->pInstance;
    }


    template<>
    void FEventsComponentEditor::onUpdate<CTransform>(const Entity& entity) {
        FEventsComponentEntity::onUpdate<CTransform>(entity);
    }


    template<>
    void FEventsComponentEditor::onAdd<CRenderable>(const Entity& entity) {
        FEventsComponentEntity::onAdd<CRenderable>(entity);
    }

    static void loadTexture2DAndAssignToComponent(FMaterialManager* pMaterialManager,
                                                  CRenderable& cRenderable,
                                                  const std::string& path) {
        const std::string texture2DPath =
                FFileManager::getRelativePath(FProjectManager::getProject().getAssetsPath(), path);
        FTex2DInfo info;
        info.path = texture2DPath;
        info.id = FProjectManager::generateUniqueID();
        FMaterialTex2D* pTexture2D{ pMaterialManager->getFactory()->emplaceTex2D(info.path) };
        pTexture2D->passInfo(info);
        cRenderable.material.type = EMaterialType::TEX2D;
        cRenderable.material.index = pTexture2D->getIndex();
    }

    template<>
    void FEventsComponentEditor::onUpdate<CRenderable>(const Entity& entity) {
        const auto& cEvent{ entity.getComponent<CEvent>() };

        if(cEvent.eventUpdateType == EEventType::RENDERABLE_TEX2D_LOAD) {
            loadTexture2DAndAssignToComponent(s_pMaterialManager,
                                              entity.getComponent<CRenderable>(),
                                              *cEvent.pFilesystemDialogInfo->pPath);
        }

        FEventsComponentEntity::onUpdate<CRenderable>(entity);
    }

    template<>
    void FEventsComponentEditor::onRemove<CRenderable>(const Entity& entity) {
        FEventsComponentEntity::onRemove<CRenderable>(entity);
    }

    template<>
    void FEventsComponentEditor::onAdd<CPointLight>(const Entity& entity) {
        FEventsComponentEntity::onAdd<CPointLight>(entity);
    }

    template<>
    void FEventsComponentEditor::onUpdate<CPointLight>(const Entity& entity) {
        FEventsComponentEntity::onUpdate<CPointLight>(entity);
    }

    template<>
    void FEventsComponentEditor::onRemove<CPointLight>(const Entity& entity) {
        FEventsComponentEntity::onRemove<CPointLight>(entity);
    }


    template<>
    void FEventsComponentEditor::onAdd<CCamera>(const Entity& entity) {
        FEventsComponentEntity::onAdd<CCamera>(entity);
    }

    template<>
    void FEventsComponentEditor::onUpdate<CCamera>(const Entity& entity) {

    }

    template<>
    void FEventsComponentEditor::onRemove<CCamera>(const Entity& entity) {
        FEventsComponentEntity::onRemove<CCamera>(entity);
    }


    static std::string getFilename(const std::string& path) {
        return path.substr(path.find_last_of("/\\") + 1);
    }

    static void openInScriptEditor(FScriptEditorWidget* pScriptWidget, CPythonScript& cPythonScript) {
        // remember that is gets relative path from .exe file and saves it
        std::string sourceCode;
        if(!FFileManager::isValidPath(cPythonScript.scriptsPath)) {
            MARLOG_WARN(ELoggerType::EDITOR,
                        "Path is not pointing to file, so it wont be displayed at editor -> {}",
                        cPythonScript.scriptsPath);
            return;
        }
        FFileManager::loadFile(sourceCode, cPythonScript.scriptsPath.c_str());
        pScriptWidget->setEditorTitle(getFilename(cPythonScript.scriptsPath));
        pScriptWidget->setEditorCode(sourceCode);
        pScriptWidget->setPathToScript(cPythonScript.scriptsPath);
    }

    static void assignScriptPathToComponentAndOpenInEditor(FScriptEditorWidget* pScriptWidget,
                                                           CPythonScript& cPythonScript,
                                                           const FFilesystemDialogInfo& dialogInfo) {
        cPythonScript.scriptsPath =
                FFileManager::getRelativePath(FProjectManager::getProject().getAssetsPath(),
                                              *dialogInfo.pPath);
        openInScriptEditor(pScriptWidget, cPythonScript);
    }

    template<>
    void FEventsComponentEditor::onAdd<CPythonScript>(const Entity& entity) {
        FEventsComponentEntity::onAdd<CPythonScript>(entity);
    }

    template<>
    void FEventsComponentEditor::onUpdate<CPythonScript>(const Entity& entity) {
        const auto& cEvent{ entity.getComponent<CEvent>() };

        if(cEvent.eventUpdateType == EEventType::PYTHONSCRIPT_CREATE_ASSIGN) {
            FFileManager::saveAsFile(s_pScriptWidget->getDefaultEditorSourceCode(),
                                     cEvent.pFilesystemDialogInfo->pPath->c_str());

            assignScriptPathToComponentAndOpenInEditor(s_pScriptWidget,
                                                       entity.getComponent<CPythonScript>(),
                                                       *cEvent.pFilesystemDialogInfo);
        }
        else if(cEvent.eventUpdateType == EEventType::PYTHONSCRIPT_ASSIGN) {
            assignScriptPathToComponentAndOpenInEditor(s_pScriptWidget,
                                                       entity.getComponent<CPythonScript>(),
                                                       *cEvent.pFilesystemDialogInfo);
        }
        else if(cEvent.eventUpdateType == EEventType::PYTHONSCRIPT_OPEN) {
            openInScriptEditor(s_pScriptWidget, entity.getComponent<CPythonScript>());
        }

        entity.removeComponent<CEvent>();
    }

    template<>
    void FEventsComponentEditor::onRemove<CPythonScript>(const Entity& entity) {
        // TODO: handle script editor widget clear during component removal
        FEventsComponentEntity::onRemove<CPythonScript>(entity);
    }

}
