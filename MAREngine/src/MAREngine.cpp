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


#include "MAREngine.h"
#include "Core/scripting/PythonInterpreter.h"
#include "Logging/Logger.h"
// RENDER API
#include "Core/graphics/public/Renderer.h"
#include "Core/graphics/public/RenderManager.h"
#include "Core/graphics/public/BatchManager.h"
#include "Core/graphics/public/MeshManager.h"
#include "Core/graphics/public/MaterialManager.h"
#include "Core/graphics/private/OpenGL/GraphicsOpenGL.h"
#include "Core/graphics/private/OpenGL/RendererOpenGL.h"
// SCENE
#include "Core/ecs/Scene.h"
#include "Core/ecs/SceneManagerEditor.h"
#include "Core/ecs/Entity/EventsCameraEntity.h"
#include "Core/ecs/Entity/EventsComponentEntity.h"
// FILESYSTEM
#include "Core/filesystem/public/FileManager.h"
// EDITOR
#include "Editor/public/ServiceManagerEditor.h"
#include "Editor/public/ServiceLocatorEditor.h"
#include "Editor/public/EventsEntityEditor.h"
#include "Editor/public/EventsComponentEditor.h"
// WINDOW
#include "Window/Window.h"
#include "Window/GLFW/WindowGLFW.h"


namespace marengine {


    void MAREngine::initAtStartup() {
        static bool initializedAtStartup{ false };
        if(initializedAtStartup) {
            return;
        }

        FLogger::init();
		FPythonInterpreter::init();

        FEngineState::passEngine(this);

        if(!m_engineConfig.exists()) {
            m_engineConfig.createDefault();
        }

        m_engineConfig.load();

        FProjectManager::init(getProjectManager());

        initializedAtStartup = true;
    }


    template<EWindowContextType TWindowType, ERenderContextType TRenderType, EEditorContextType TEditorType>
    static void run(MAREngine* pEngine);

    template<EWindowContextType TWindowType, ERenderContextType TRenderType>
    static void run(MAREngine* pEngine);

    template<EWindowContextType TWindowType, ERenderContextType TRenderType, EEditorContextType TEditorType>
    static auto createWindowType();

    template<EWindowContextType TWindowType, ERenderContextType TRenderType>
    static auto createWindowType();

    template<ERenderContextType TRenderType>
    static auto createRenderContextType();

    template<ERenderContextType TRenderType>
    static auto createRenderCommandsType();


    void MAREngine::buildAndRun() {
        setRestart();
        while(isGoingToRestart()) {
            setNoRestart();
            run<EWindowContextType::GLFW, ERenderContextType::OPENGL, EEditorContextType::IMGUI>(this);
            //run<EWindowContextType::GLFW, ERenderContextType::OPENGL>(this);
        }
    }

    template<EWindowContextType TWindowType, ERenderContextType TRenderType, EEditorContextType TEditorType>
    void run(MAREngine* pEngine) {
        auto window{ createWindowType<TWindowType, TRenderType, TEditorType>() };

        const bool isWindowLibraryInitialized = window.initializeLibrary();
        if(!isWindowLibraryInitialized) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window library!");
            return;
        }

        const bool isWindowCreated{
            window.open(1600, 900, FProjectManager::getProject().getWindowName().c_str())
        };
        if(!isWindowCreated) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window!");
            return;
        }

        // RENDER API
        FRenderStatistics renderStatistics;
        FBatchManager batchManager;
        FMeshManager meshManager;
        FRenderManager renderManager;
        FMaterialManager materialManager;
        // OPENGL RENDER API
        auto renderContext{ createRenderContextType<TRenderType>() };
        auto renderCommands{ createRenderCommandsType<TRenderType>() };
        // SCENE
        FSceneManagerEditor sceneManager;
        // EDITOR
        FServiceManagerEditor serviceManagerEditor;
        FServiceLocatorEditor serviceLocatorEditor;

        renderContext.create(&window);
        renderManager.create(&renderContext);
        materialManager.create(&renderContext);
        batchManager.create(&renderManager, meshManager.getStorage(), materialManager.getStorage());

        FEngineConfig* pEngineConfig{ pEngine->getEngineConfig() };
        const FMinimalProjectInfo* pProjectInfo{ pEngineConfig->getProjectInfo("DefaultProject") };
        FProject& project{ FProjectManager::loadProject(pProjectInfo) };

        Scene* pScene{ project.getSceneToLoad() };

        FFramebuffer* pFramebufferViewport{ renderManager.getViewportFramebuffer() };
        pFramebufferViewport->setClearColor(pScene->getBackground());

        FPipelineStorage* pPipelineStorage{ renderContext.getPipelineStorage() };

        FEventsCameraEntity::passSceneManager(&sceneManager);
        FEventsCameraEntity::passRenderManager(&renderManager);

        FEventsComponentEntity::passSceneManager(&sceneManager);
        FEventsComponentEntity::passRenderManager(&renderManager);
        FEventsComponentEntity::passBatchManager(&batchManager);
        FEventsComponentEntity::passMeshManager(&meshManager);

        sceneManager.initialize(pScene, &batchManager, &meshManager);

        serviceLocatorEditor.registerServices(&window, &sceneManager, &renderStatistics,
                                              &meshManager, &renderManager, &materialManager);

        serviceLocatorEditor.create<TEditorType>();
        serviceManagerEditor.create<TEditorType>(&serviceLocatorEditor);

        FEventsEntityEditor::create(&serviceLocatorEditor);
        FEventsComponentEditor::create(&serviceLocatorEditor);

        serviceManagerEditor.onCreate();

        while(!window.isGoingToClose() && !pEngine->isGoingToRestart()) {
            renderStatistics.reset();

            pFramebufferViewport->clear();

            const uint32_t countColor{ pPipelineStorage->getCountColorMesh() };
            for(uint32_t i = 0; i < countColor; i++) {
                renderCommands.draw(pFramebufferViewport, pPipelineStorage->getColorMesh(i));
            }

            const uint32_t countTex2D{ pPipelineStorage->getCountTex2DMesh() };
            for(uint32_t i = 0; i < countTex2D; i++) {
                renderCommands.draw(pFramebufferViewport, pPipelineStorage->getTex2DMesh(i));
            }

            sceneManager.update();
            serviceManagerEditor.onUpdate();

            window.swapBuffers();
        }

        renderManager.reset();
        renderStatistics.reset();
        batchManager.reset();
        meshManager.reset();
        renderContext.close();

        sceneManager.close();

        serviceManagerEditor.onDestroy();
        serviceLocatorEditor.close();

        window.terminateLibrary();
    }

    template<EWindowContextType TWindowType, ERenderContextType TRenderType>
    void run(MAREngine* pEngine) {
        // TODO: fix only runtime mode // game
        auto window{ createWindowType<TWindowType, TRenderType>() };

        const bool isWindowLibraryInitialized = window.initializeLibrary();
        if(!isWindowLibraryInitialized) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window library!");
            return;
        }

        const bool isWindowCreated = window.open(1600, 900, pEngine->getWindowName().c_str());
        if(!isWindowCreated) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window!");
            return;
        }

        // RENDER API
        FRenderStatistics renderStatistics;
        FBatchManager batchManager;
        FMeshManager meshManager;
        FRenderManager renderManager;
        FMaterialManager materialManager;
        // OPENGL RENDER API
        auto renderContext{ createRenderContextType<TRenderType>() };
        auto renderCommands{ createRenderCommandsType<TRenderType>() };
        // SCENE
        FSceneManagerEditor sceneManager;

        renderContext.create(&window);
        renderManager.create(&renderContext);
        materialManager.create(&renderContext);
        batchManager.create(&renderManager, meshManager.getStorage(), materialManager.getStorage());

        Scene* pScene =
                FSceneDeserializer::loadSceneFromFile(FProjectManager::getSceneToLoadAtStartup());

        FFramebuffer* pFramebufferViewport{ renderManager.getViewportFramebuffer() };
        pFramebufferViewport->setClearColor(pScene->getBackground());

        window.setClearColor(pScene->getBackground());

        FPipelineStorage* pPipelineStorage{ renderContext.getPipelineStorage() };

        FEventsCameraEntity::passSceneManager(&sceneManager);
        FEventsCameraEntity::passRenderManager(&renderManager);

        FEventsComponentEntity::passSceneManager(&sceneManager);
        FEventsComponentEntity::passRenderManager(&renderManager);
        FEventsComponentEntity::passBatchManager(&batchManager);
        FEventsComponentEntity::passMeshManager(&meshManager);

        sceneManager.initialize(pScene, &batchManager, &meshManager);

        while(!window.isGoingToClose() && !pEngine->isGoingToRestart()) {
            renderStatistics.reset();
            window.clear();

            const uint32_t countColor{ pPipelineStorage->getCountColorMesh() };
            for(uint32_t i = 0; i < countColor; i++) {
                renderCommands.draw(pPipelineStorage->getColorMesh(i));
            }

            const uint32_t countTex2D{ pPipelineStorage->getCountTex2DMesh() };
            for(uint32_t i = 0; i < countTex2D; i++) {
                renderCommands.draw(pPipelineStorage->getTex2DMesh(i));
            }

            sceneManager.update();

            window.swapBuffers();
        }

        renderManager.reset();
        renderStatistics.reset();
        batchManager.reset();
        meshManager.reset();
        renderContext.close();

        sceneManager.close();

        window.terminateLibrary();
    }

	bool MAREngine::isGoingToRestart() const {
		return m_shouldRestart;
	}

    FProjectManager* MAREngine::getProjectManager() const {
        return (FProjectManager*)&m_projectManager;
    }

    FEngineConfig* MAREngine::getEngineConfig() const {
        return (FEngineConfig*)&m_engineConfig;
    }

	void MAREngine::setRestart() {
		m_shouldRestart = true; 
	}

	void MAREngine::setNoRestart() {
		m_shouldRestart = false; 
	}


	MAREngine* FEngineState::s_pEngine{ nullptr };

    void FEngineState::passEngine(MAREngine* pEngine) {
        if(s_pEngine == nullptr) {
            s_pEngine = pEngine;
        }
        else {
            MARLOG_WARN(ELoggerType::NORMAL, "Engine instance has been already set!");
        }
    }

    void FEngineState::setRestart() {
        s_pEngine->setRestart();
    }

    void FEngineState::setNoRestart() {
        s_pEngine->setNoRestart();
    }

    bool FEngineState::isGoingToRestart() {
        return s_pEngine->isGoingToRestart();
    }



    template<ERenderContextType TRenderType>
    auto createRenderContextType() {
        if constexpr (TRenderType == ERenderContextType::OPENGL) {
            return FRenderContextOpenGL();
        }
        else {
            return int(5);
        }
    }

    template<ERenderContextType TRenderType>
    auto createRenderCommandsType() {
        if constexpr (TRenderType == ERenderContextType::OPENGL) {
            return FRenderCommandOpenGL();
        }
        else {
            return int(5);
        }
    }

    template<EWindowContextType TWindowType, ERenderContextType TRenderType, EEditorContextType TEditorType>
    auto createWindowType() {
        if constexpr (
                TWindowType == EWindowContextType::GLFW &&
                TEditorType == EEditorContextType::IMGUI &&
                TRenderType == ERenderContextType::OPENGL) {
            return FWindowGLFWImGui();
        }
        else {
            return int(5);
        }
    }

    template<EWindowContextType TWindowType, ERenderContextType TRenderType>
    auto createWindowType() {
        if constexpr (
                TWindowType == EWindowContextType::GLFW &&
                TRenderType == ERenderContextType::OPENGL) {
            return FWindowGLFWImGui();
        }
        else {
            return int(5);
        }
    }


}
