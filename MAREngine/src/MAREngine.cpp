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
#include "Window/IWindow.h"
#include "LayerStack/layers/RenderLayer.h"
#include "LayerStack/layers/SceneLayer.h"
#include "LayerStack/layers/EditorLayer.h"


namespace marengine {


    void MAREngine::initAtStartup(std::string projectName, std::string sceneToLoadAtStartup) {
        static bool initializedAtStartup{ false };
        if(initializedAtStartup) {
            return;
        }

        FLogger::init();
        FProjectManager::init(&m_projectManager, std::move(projectName), std::move(sceneToLoadAtStartup));
		FPythonInterpreter::init();

        initializedAtStartup = true;
        setRestart();
    }

    void MAREngine::buildAndRun(IMAREngineBuilder* pBuilder) {
        setNoRestart();

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
        sceneLayer->create(getStartupSceneFilename(), renderLayer->getRenderManager());
        editorLayer->create(window, sceneLayer->getSceneManager(), renderLayer->getRenderStats());

        layerStack.pushLayer(renderLayer);
        layerStack.pushLayer(sceneLayer);
        layerStack.pushLayer(editorLayer);

        while(!window->isGoingToClose() && !shouldEngineRestart()) {
            layerStack.begin();
            layerStack.update();
            layerStack.end();

            window->swapBuffers();
        }

        layerStack.close();
        window->terminateLibrary();
    }

	bool MAREngine::shouldEngineRestart() const {
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


}
