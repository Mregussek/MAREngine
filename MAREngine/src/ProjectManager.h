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


#ifndef MAR_ENGINE_PROJECT_MANAGER_H
#define MAR_ENGINE_PROJECT_MANAGER_H


#include "mar.h"


namespace marengine {



	/**
	 * @struct ProjectInfo ProjectManager.h "ProjectManager.h"
	 * @brief ProjectInfo is a structure containg all information about currently
	 * running project on MAREngine. It contains all members that are mandatory for proper
	 * MAREngine.
	 */
	struct ProjectInfo {

		std::string projectName;
		std::string projectPath;
		std::string assetsPath;
		std::string scenesPath;
		std::string sceneToLoadAtStartup;
		std::string windowName;

	};

	
	class ProjectManager {
	public:

		static ProjectManager* Instance;

		void fillProjectInfo(std::string newProjectName, std::string newSceneToLoadAtStartup);
		
		void addNewSceneToCurrentProject(const std::string& newSceneFilenameToProject);

		void setNewSceneToLoad(const std::string& sceneFilenameToLoad);

		MAR_NO_DISCARD const ProjectInfo& getProjectInfo() const;

		MAR_NO_DISCARD const std::string& getProjectName() const;
		MAR_NO_DISCARD const std::string& getProjectPath() const;
		MAR_NO_DISCARD const std::string& getAssetsPath() const;
		MAR_NO_DISCARD const std::string& getScenesPath() const;
		MAR_NO_DISCARD const std::string& getSceneToLoadAtStartup() const;
		MAR_NO_DISCARD const std::string& getWindowName() const;

	private:

		ProjectInfo m_projectInfo;

	};


}


#endif // !MAR_ENGINE_PROJECT_MANAGER_H
