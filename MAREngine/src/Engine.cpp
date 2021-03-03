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


#include "Engine.h"
#include "Debug/Log.h"
#include "Core/scripting/PythonScript.h"


namespace marengine {

		
	MAREngine* MAREngine::Instance{ nullptr };


	void MAREngine::initialize(std::string projectName, std::string sceneToLoadAtStartup) {
		Instance = this;
		m_projectManager.Instance = &m_projectManager;
		m_projectManager.fillProjectInfo(projectName, sceneToLoadAtStartup);

		MAR_LOG_INIT();

		PythonScript::appendCurrentPath();
	}

	MAR_NO_DISCARD const bool MAREngine::shouldEngineRestart() const {
		return m_shouldRestart;
	}

	MAR_NO_DISCARD const std::string& MAREngine::getStartupSceneFilename() const {
		return m_projectManager.getProjectInfo().sceneToLoadAtStartup;
	}

	MAR_NO_DISCARD const std::string& MAREngine::getWindowName() const {
		return m_projectManager.getProjectInfo().windowName;
	}

	void MAREngine::setRestart() {
		m_shouldRestart = true; 
	}

	void MAREngine::setNoRestart() {
		m_shouldRestart = false; 
	}


}
