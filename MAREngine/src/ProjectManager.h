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
#include "Core/ecs/Scene.h"


namespace marengine {

    class Scene;


	/**
	 * @struct ProjectInfo ProjectManager.h "ProjectManager.h"
	 * @brief ProjectInfo is a structure containing all information about currently running project
	 * on MAREngine. It contains all members that are mandatory for proper MAREngine run.
	 */
	struct FProjectInfo {

	    std::string projectVersion;
        std::string projectCfg;
		std::string projectName;
		std::string projectPath;
		std::string assetsPath;
		std::string scenesPath;
		std::string sceneToLoadAtStartup;
		std::string windowName;

	};


	struct FMinimalProjectInfo {
	    std::string projectName;
	    std::string projectPath;
	};


	class FProject {
	public:

	    void setProjectName(const std::string& projectName);
	    const std::string& getProjectName() const;

	    void setProjectPath(const std::string& projectPath);
	    const std::string& getProjectPath() const;
	    const std::string& getProjectVersion() const;
	    const std::string& getProjectConfigPath() const;
	    const std::string& getAssetsPath() const;
	    const std::string& getScenesPath() const;
	    const std::string& getWindowName() const;
        Scene* getSceneToLoad();

	    void setSceneStartup(const std::string& startupScene);
	    void setProjectVersion(const std::string& version);
	    void updateWindowName();

	    Scene* addScene(const std::string& sceneName);
	    void removeScene(Scene* pScene);

	    void useScene(Scene* pScene) const;
	    void useScene(uint32 index) const;

	private:

	    FProjectInfo m_projectInfo;
	    std::vector<Scene> m_scenes;

	};

	
	class FProjectManager {

	    friend class MAREngine;

	public:

		static uint32 generateUniqueID();

		MAR_NO_DISCARD static FProject& getProject();

		static FProject& loadProject(const FMinimalProjectInfo* pProjectInfo);

	private:

        static void init(FProjectManager* pProjectManagerInstance);


	    static FProjectManager* s_pInstance;
        FProject m_project;

	};


}


#endif // !MAR_ENGINE_PROJECT_MANAGER_H
