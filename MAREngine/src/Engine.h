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


#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


#include "mar.h"
#include "ProjectManager.h"
#include "Debug/Log.h"


namespace marengine {


	class MAREngine {
	public:

		static MAREngine* Instance;


		MAR_NO_DISCARD bool shouldEngineRestart() const;
		MAR_NO_DISCARD const std::string& getStartupSceneFilename() const;
		MAR_NO_DISCARD const std::string& getWindowName() const;

		void setRestart();
		void setNoRestart();

		void initialize(std::string projectName, std::string sceneToLoadAtStartup);
		
	private:

		ProjectManager m_projectManager;
		LoggerType<std::shared_ptr<spdlog::logger>> m_logger;

		bool m_shouldRestart{ false };

	};


}

#endif // !MAR_ENGINE_MAIN_H