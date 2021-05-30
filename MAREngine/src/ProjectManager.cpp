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
#include "Core/filesystem/public/FileManager.h"
#include "Core/ecs/Scene.h"


namespace marengine {


    FProjectManager* FProjectManager::s_pInstance{ nullptr };


	void FProjectManager::init(FProjectManager* pProjectManagerInstance) {
        s_pInstance = pProjectManagerInstance;

        getProject().setProjectName("NoProject");
        getProject().setProjectPath("NoPath");
	}

    FProject& FProjectManager::loadProject(const FMinimalProjectInfo* pProjectInfo) {
        getProject().setProjectName(pProjectInfo->projectName);
        getProject().setProjectPath(pProjectInfo->projectPath);
        getProject().updateWindowName();

        // should load project.cfg and retrieve info about current scene, all meshes, textures etc.
        if(FFileManager::isValidPath(getProject().getProjectConfigPath())) {
            FFileDeserializer::loadProjectFromFile(&getProject(), getProject().getProjectConfigPath());
        }

        return getProject();
	}

    static void eraseSubstring(std::string& mainStr, const std::string& toErase) {
        auto pos{ mainStr.find(toErase) };
        if(pos != std::string::npos) {
            mainStr.erase(pos, toErase.length());
        }
    }

	uint32 FProjectManager::generateUniqueID() {
	    static uint32 id{ 0 };
	    id++;
	    return id;
	}

    FProject& FProjectManager::getProject() {
	    return s_pInstance->m_project;
	}


    void FProject::setProjectName(const std::string& projectName) {
        m_projectInfo.projectName = projectName;
	}

    void FProject::setProjectPath(const std::string& projectPath) {
        m_projectInfo.projectPath = projectPath;
        m_projectInfo.assetsPath =  projectPath + "Assets/";
        m_projectInfo.scenesPath =  projectPath + "Scenes/";
        m_projectInfo.projectCfg = projectPath + "project.cfg";
    }

    void FProject::setSceneStartup(const std::string& startupScene) {
	    m_projectInfo.sceneToLoadAtStartup = startupScene;
	}

    void FProject::updateWindowName() {
        m_projectInfo.windowName = getProjectName() + " --- MAREngine";
	}

    void FProject::setProjectVersion(const std::string& version) {
	    m_projectInfo.projectVersion = version;
	}

    Scene* FProject::addScene(const std::string& sceneName) {
        return &m_scenes.emplace_back(sceneName);
	}

    void FProject::removeScene(Scene* pScene) {

	}

    void FProject::useScene(Scene* pScene) const {

    }

    void FProject::useScene(uint32 index) const {

	}

    const std::string& FProject::getProjectConfigPath() const {
        return m_projectInfo.projectCfg;
    }

    const std::string& FProject::getProjectVersion() const {
	    return m_projectInfo.projectVersion;
	}

    const std::string& FProject::getProjectName() const {
        return m_projectInfo.projectName;
    }

    const std::string& FProject::getProjectPath() const {
        return m_projectInfo.projectPath;
    }

    const std::string& FProject::getAssetsPath() const {
        return m_projectInfo.assetsPath;
    }

    const std::string& FProject::getScenesPath() const {
        return m_projectInfo.scenesPath;
    }

    Scene* FProject::getSceneToLoad() {
        auto doesSceneToLoadExists =
                [&sceneToLoad = std::as_const(m_projectInfo.sceneToLoadAtStartup)]
                (Scene& scene) {
            return scene.getName() == sceneToLoad;
        };
	    auto it = std::find_if(m_scenes.begin(), m_scenes.end(), doesSceneToLoadExists);
        if(it != m_scenes.cend()) {
            return &(*it);
        }

        return nullptr;
    }

    const std::string& FProject::getWindowName() const {
        return m_projectInfo.windowName;
    }


}
