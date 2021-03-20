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


#include "ProjectManager.h"

#include <utility>


namespace marengine {


    FProjectManager* FProjectManager::s_pInstance{ nullptr };


	void FProjectManager::init(FProjectManager* pProjectManagerInstance, std::string newProjectName, std::string newSceneToLoadAtStartup) {
        s_pInstance = pProjectManagerInstance;

		setProjectName(std::move(newProjectName));
        setSceneToLoadAtStartup(std::move(newSceneToLoadAtStartup));
	}

	void FProjectManager::addNewSceneToCurrentProject(std::string newSceneFilenameToProject) {

		setNewSceneToLoad(std::move(newSceneFilenameToProject));

	}

	void FProjectManager::setNewSceneToLoad(std::string sceneFilenameToLoad) {

        setSceneToLoadAtStartup(std::move(sceneFilenameToLoad));

	}

	const FProjectInfo& FProjectManager::getProjectInfo() {
		return s_pInstance->m_projectInfo;
	}

	const std::string& FProjectManager::getProjectName() {
		return s_pInstance->m_projectInfo.projectName;
	}

	const std::string& FProjectManager::getProjectPath() {
		return s_pInstance->m_projectInfo.projectPath;
	}

	const std::string& FProjectManager::getAssetsPath() {
		return s_pInstance->m_projectInfo.assetsPath;
	}

	const std::string& FProjectManager::getScenesPath() {
		return s_pInstance->m_projectInfo.scenesPath;
	}

	const std::string& FProjectManager::getSceneToLoadAtStartup() {
		return s_pInstance->m_projectInfo.sceneToLoadAtStartup;
	}

	const std::string& FProjectManager::getWindowName() {
		return s_pInstance->m_projectInfo.windowName;
	}

    void FProjectManager::setProjectName(std::string newProjectName) {
        s_pInstance->m_projectInfo.projectName = std::move(newProjectName);

        s_pInstance->m_projectInfo.projectPath = s_pInstance->m_projectInfo.projectName + "/";
        s_pInstance->m_projectInfo.assetsPath =  s_pInstance->m_projectInfo.projectPath + "Assets/";
        s_pInstance->m_projectInfo.scenesPath =  s_pInstance->m_projectInfo.projectPath + "Scenes/";

        setWindowName();
	}

    void FProjectManager::setSceneToLoadAtStartup(std::string newSceneToLoad) {
        s_pInstance->m_projectInfo.sceneToLoadAtStartup = s_pInstance->m_projectInfo.scenesPath + newSceneToLoad;
        setWindowName();
	}

	void FProjectManager::setWindowName() {
        s_pInstance->m_projectInfo.windowName =
                s_pInstance->m_projectInfo.projectName
                + " --- "
                + s_pInstance->m_projectInfo.sceneToLoadAtStartup
                + " --- MAREngine";
	}


}
