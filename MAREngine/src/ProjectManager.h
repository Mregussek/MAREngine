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
	 * @brief ProjectInfo is a structure containing all information about currently running project
	 * on MAREngine. It contains all members that are mandatory for proper MAREngine run.
	 */
	struct FProjectInfo {

	    std::string absolutePath;
		std::string projectName;
		std::string projectPath;
		std::string assetsPath;
		std::string scenesPath;
		std::string sceneToLoadAtStartup;
		std::string windowName;

	};

	
	class FProjectManager {

	    friend class MAREngine;

	public:

		static void addNewSceneToCurrentProject(const std::string& newSceneFilenameToProject);
		static void setNewSceneToLoad(const std::string& sceneFilenameToLoad);

		static void fillProjectInfo(const std::string& scenePath, const std::string& sceneFilename);
		static void retrieveProjectInfo(const std::string& scenePath, const std::string& sceneFilename);

		MAR_NO_DISCARD static const FProjectInfo& getProjectInfo();

		MAR_NO_DISCARD static const std::string& getAbsolutePath();
		MAR_NO_DISCARD static const std::string& getProjectName();
		MAR_NO_DISCARD static const std::string& getProjectPath();
		MAR_NO_DISCARD static const std::string& getAssetsPath();
		MAR_NO_DISCARD static const std::string& getScenesPath();
		MAR_NO_DISCARD static const std::string& getSceneToLoadAtStartup();
		MAR_NO_DISCARD static const std::string& getWindowName();

	private:

        static void init(FProjectManager* pProjectManagerInstance, std::string newProjectName,
                         const std::string& newSceneToLoadAtStartup);

	    static void setProjectName(std::string newProjectName);
	    static void setSceneToLoadAtStartup(const std::string& newSceneToLoad);
	    static void setAbsolutePath();
        static void setWindowName();


	    static FProjectManager* s_pInstance;
        FProjectInfo m_projectInfo;

	};


}


#endif // !MAR_ENGINE_PROJECT_MANAGER_H
