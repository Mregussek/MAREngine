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


namespace marengine {

    class IMAREngineBuilder;


	class MAREngine {
	public:

		MAR_NO_DISCARD bool isGoingToRestart() const;
		MAR_NO_DISCARD const std::string& getStartupSceneFilename() const;
		MAR_NO_DISCARD const std::string& getWindowName() const;

		void setRestart();
		void setNoRestart();

		void initAtStartup(std::string projectName, const std::string& sceneToLoadAtStartup);

		void buildAndRun();

	private:

        FProjectManager m_projectManager;

		bool m_shouldRestart{ false };

	};


	class FEngineState {
	public:

	    static void passEngine(MAREngine* pEngine);

	    static void setRestart();
	    static void setNoRestart();
	    MAR_NO_DISCARD static bool isGoingToRestart();

	private:

	    static MAREngine* s_pEngine;

	};


}

#endif // !MAR_ENGINE_MAIN_H