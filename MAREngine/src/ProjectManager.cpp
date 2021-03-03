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


namespace marengine {


	ProjectManager* ProjectManager::Instance{ nullptr };


	void ProjectManager::fillProjectInfo(std::string newProjectName, std::string newSceneToLoadAtStartup) {

		m_projectInfo.projectName = newProjectName;
		m_projectInfo.projectPath = m_projectInfo.projectName + "/";
		m_projectInfo.assetsPath = m_projectInfo.projectPath + "Assets/";
		m_projectInfo.scenesPath = m_projectInfo.projectPath + "Scenes/";
		m_projectInfo.sceneToLoadAtStartup = m_projectInfo.scenesPath + newSceneToLoadAtStartup;
		m_projectInfo.windowName = m_projectInfo.projectName + " --- " + newSceneToLoadAtStartup + " --- MAREngine";

	}

	void ProjectManager::addNewSceneToCurrentProject(const std::string& newSceneFilenameToProject) {

		setNewSceneToLoad(newSceneFilenameToProject);

	}

	void ProjectManager::setNewSceneToLoad(const std::string& sceneFilenameToLoad) {

		m_projectInfo.sceneToLoadAtStartup = m_projectInfo.scenesPath + sceneFilenameToLoad;
		m_projectInfo.windowName = m_projectInfo.projectName + " --- " + sceneFilenameToLoad + " --- MAREngine";

	}

	const ProjectInfo& ProjectManager::getProjectInfo() const {
		return m_projectInfo;
	}

	const std::string& ProjectManager::getProjectName() const {
		return m_projectInfo.projectName;
	}

	const std::string& ProjectManager::getProjectPath() const {
		return m_projectInfo.projectPath;
	}

	const std::string& ProjectManager::getAssetsPath() const {
		return m_projectInfo.assetsPath;
	}

	const std::string& ProjectManager::getScenesPath() const {
		return m_projectInfo.scenesPath;
	}

	const std::string& ProjectManager::getSceneToLoadAtStartup() const {
		return m_projectInfo.sceneToLoadAtStartup;
	}

	const std::string& ProjectManager::getWindowName() const {
		return m_projectInfo.windowName;
	}


}
