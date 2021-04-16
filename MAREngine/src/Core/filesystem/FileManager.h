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
	 * @class FFileManager FileManager.h "Core/filesystem/FileManager.h"
	 * @brief Class responsible for working with files, needed for play mode as in editor mode.
	 */
	class FFileManager {
	public:

		/**
		 * @brief Method loads file from given path and saves it into given 
		 * stringToFill variable.
		 * @param stringToFill string variable, at which loaded file will be saved
		 * @param path path, from which file will be loaded.
		 */
		static void loadFile(std::string& stringToFill, const char* path);

		/**
		 * @brief Method saves file given at sourceCode variable to given path.
		 * @param sourceCode string variable containg all the sourceCode of the script
		 * @param path path at which script will be saved.
		 */
		static void saveAsFile(const std::string& sourceCode, const char* path);

		// TODO: add docs isContainingExtension
		static bool isContainingExtension(const std::string& path, const std::string& extension);

		// TODO: add docs getAbsolutePath
		MAR_NO_DISCARD static std::string getAbsolutePath(const std::string& relativePath);

		// TODO: add docs isValidPath
		MAR_NO_DISCARD static bool isValidPath(const std::string& path);

		MAR_NO_DISCARD static std::string getRelativePath(const std::string& absolutePath,
                                                          const std::string& path);

	};


}


#endif // !MAR_ENGINE_F_SCRIPTS_FILE_MANAGER_H
