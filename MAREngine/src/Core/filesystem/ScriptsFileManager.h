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


#ifndef MAR_ENGINE_F_SCRIPTS_FILE_MANAGER_H
#define MAR_ENGINE_F_SCRIPTS_FILE_MANAGER_H


#include "../../mar.h"


namespace marengine {


	/**
	 * @class FScriptsFileManager ScriptsFileManager.h "Core/filesystem/ScriptsFileManager.h"
	 * @brief Class responsible for working with scripts file, needed for play mode in MAREngine.
	 */
	class FScriptsFileManager {
	public:

		/**
		 * @brief Method loads script from given path and saves it into given 
		 * stringToFill variable.
		 * @param stringToFill string variable, at which loaded script will be saved
		 * @param path path, from which script will be loaded.
		 */
		static void loadScript(std::string& stringToFill, const char* path);

		/**
		 * @brief Method saves script given at sourceCode variable to given path.
		 * @param sourceCode string variable containg all the sourceCode of the script
		 * @param path path at which script will be saved.
		 */
		static void saveScript(const std::string& sourceCode, const char* path);

	};


}


#endif // !MAR_ENGINE_F_SCRIPTS_FILE_MANAGER_H
