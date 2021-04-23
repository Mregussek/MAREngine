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
#include "Core/filesystem/FileManager.h"


namespace marengine {


    FProjectManager* FProjectManager::s_pInstance{ nullptr };


	void FProjectManager::init(FProjectManager* pProjectManagerInstance, std::string newProjectName,
                            const std::string& newSceneToLoadAtStartup) {
        s_pInstance = pProjectManagerInstance;

		setProjectName(std::move(newProjectName));
        setSceneToLoadAtStartup(newSceneToLoadAtStartup);
        setAbsolutePath();
	}

    void FProjectManager::setProjectName(std::string newProjectName) {
        s_pInstance->m_projectInfo.projectName = std::move(newProjectName);
        s_pInstance->m_projectInfo.projectPath = getProjectName() + "/";
        s_pInstance->m_projectInfo.assetsPath =  getProjectPath() + "Assets/";
        s_pInstance->m_projectInfo.scenesPath =  getProjectPath() + "Scenes/";

        setWindowName();
    }

    void FProjectManager::setSceneToLoadAtStartup(const std::string& newSceneToLoad) {
        s_pInstance->m_projectInfo.sceneToLoadAtStartup = getScenesPath() + newSceneToLoad;
        setWindowName();
    }

    void FProjectManager::setWindowName() {
        s_pInstance->m_projectInfo.windowName =
                getProjectName()
                + " --- "
                + getSceneToLoadAtStartup()
                + " --- MAREngine";
    }

    static void eraseSubstring(std::string& mainStr, const std::string& toErase) {
        auto pos{ mainStr.find(toErase) };
        if(pos != std::string::npos) {
            mainStr.erase(pos, toErase.length());
        }
    }

    void FProjectManager::setAbsolutePath() {
        auto& absolutePath{ s_pInstance->m_projectInfo.absolutePath };
        auto& sceneToLoadAtStartup{ s_pInstance->m_projectInfo.sceneToLoadAtStartup };

        absolutePath = FFileManager::getAbsolutePath(sceneToLoadAtStartup);
        const std::string sceneToLoadCopy{ sceneToLoadAtStartup };
        sceneToLoadAtStartup = absolutePath;
        eraseSubstring(absolutePath, sceneToLoadCopy);
    }

	void FProjectManager::addNewSceneToCurrentProject(const std::string& newSceneFilenameToProject) {
		setNewSceneToLoad(newSceneFilenameToProject);
	}

	void FProjectManager::setNewSceneToLoad(const std::string& sceneFilenameToLoad) {
        setSceneToLoadAtStartup(sceneFilenameToLoad);
	}

	void FProjectManager::fillProjectInfo(const std::string& scenePath, const std::string& sceneFilename) {
	    // TODO: implement fillProjectInfo
	    // method should retrieve project info from given scenePath and sceneFilename
	    // arguments must point to existing filename, create it if it doesnt exist (as we want to create new scenes)
	}

    void FProjectManager::retrieveProjectInfo(const std::string& scenePath,
                                              const std::string& sceneFilename) {
	    // TODO: implement retrieveProjectInfo
	    // method should retrieve project info from given scenePath and sceneFilename
	    // it should retrieve project info from existing filename
	}

	uint32 FProjectManager::generateUniqueID() {
	    static uint32 id{ 0 };
	    id++;
	    return id;
	}

	const FProjectInfo& FProjectManager::getProjectInfo() {
	    return s_pInstance->m_projectInfo;
	}

	const std::string& FProjectManager::getAbsolutePath() {
	    return s_pInstance->m_projectInfo.absolutePath;
	}

	const std::string& FProjectManager::getProjectName() {
		return getProjectInfo().projectName;
	}

	const std::string& FProjectManager::getProjectPath() {
		return getProjectInfo().projectPath;
	}

	const std::string& FProjectManager::getAssetsPath() {
		return getProjectInfo().assetsPath;
	}

	const std::string& FProjectManager::getScenesPath() {
		return getProjectInfo().scenesPath;
	}

	const std::string& FProjectManager::getSceneToLoadAtStartup() {
		return getProjectInfo().sceneToLoadAtStartup;
	}

	const std::string& FProjectManager::getWindowName() {
		return getProjectInfo().windowName;
	}


}
