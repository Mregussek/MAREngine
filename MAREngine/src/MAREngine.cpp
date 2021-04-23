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
#include "MAREngineBuilder.h"


namespace marengine {


    void MAREngine::initAtStartup(std::string projectName, const std::string& sceneToLoadAtStartup) {
        static bool initializedAtStartup{ false };
        if(initializedAtStartup) {
            return;
        }

        FLogger::init();
        FProjectManager::init(&m_projectManager, std::move(projectName),
                              sceneToLoadAtStartup);
		FPythonInterpreter::init();

        initializedAtStartup = true;
        FEngineState::passEngine(this);
    }

    void MAREngine::buildAndRun() {
        setRestart();
        while(isGoingToRestart()) {
            FMAREngineBuilder<
                    FWindowGLFWImGui,
                    FRenderLayerOpenGL,
                    FEditorLayerImGui
            > builder;
            setNoRestart();
            run(&builder);
        }
    }

    void MAREngine::run(IMAREngineBuilder* pBuilder) const {
        FWindow* window = pBuilder->createWindow();
        FRenderLayer* renderLayer = pBuilder->createRenderLayer();
        FSceneLayer* sceneLayer = pBuilder->createSceneLayer();
        FEditorLayer* editorLayer = pBuilder->createEditorLayer();
        FLayerStack layerStack = pBuilder->createLayerStack();

        const bool isWindowLibraryInitialized = window->initializeLibrary();
        if(!isWindowLibraryInitialized) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window library!");
            return;
        }

        const bool isWindowCreated = window->open(1600, 900, getWindowName().c_str());
        if(!isWindowCreated) {
            MARLOG_CRIT(ELoggerType::NORMAL, "Cannot initialize Window!");
            return;
        }

        renderLayer->create(window);
        sceneLayer->create(getStartupSceneFilename(),
                           renderLayer->getRenderManager(),
                           renderLayer->getBatchManager(),
                           renderLayer->getMeshManager());

        editorLayer->create(window,
                            sceneLayer->getSceneManager(),
                            renderLayer->getMeshManager(),
                            renderLayer->getRenderManager(),
                            renderLayer->getMaterialManager(),
                            renderLayer->getRenderStats());

        layerStack.pushLayer(renderLayer);
        layerStack.pushLayer(sceneLayer);
        layerStack.pushLayer(editorLayer);

        while(!window->isGoingToClose() && !isGoingToRestart()) {
            layerStack.begin();
            layerStack.update();
            layerStack.end();

            window->swapBuffers();
        }

        layerStack.close();
        window->terminateLibrary();
    }

	bool MAREngine::isGoingToRestart() const {
		return m_shouldRestart;
	}

	const std::string& MAREngine::getStartupSceneFilename() const {
		return FProjectManager::getProjectInfo().sceneToLoadAtStartup;
	}

	const std::string& MAREngine::getWindowName() const {
		return FProjectManager::getProjectInfo().windowName;
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


}
