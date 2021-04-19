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


#include "FileManager.h"
#include "../../Logging/Logger.h"


namespace marengine {


	void FFileManager::loadFile(std::string& stringToFill, const char* path) {
        // TODO: implement it
		FILE* file = fopen(path, "rb");
		fseek(file, 0, SEEK_END);
		const long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		stringToFill.resize(length);
		size_t rc = fread(stringToFill.data(), 1, length, file);
		fclose(file);
	}

	void FFileManager::saveAsFile(const std::string& sourceCode, const char* path) {
        // TODO: implement it
	}

    bool FFileManager::isContainingExtension(const std::string& path, const std::string& extension) {
        const std::string currentExtension{ path.substr(path.find_last_of('.') + 1) };
        if(currentExtension == extension) {
            MARLOG_TRACE(ELoggerType::FILESYSTEM, "Path {} contains {} extension", path, extension);
            return true;
        }

        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Path {} does not contain {} extension", path, extension);
        return false;
    }

    std::string FFileManager::getAbsolutePath(const std::string& relativePath) {
        std::string absolute{ std::filesystem::absolute(relativePath).u8string() };
        std::replace(absolute.begin(), absolute.end(), '\\', '/');
        return absolute;
	}

    std::string FFileManager::getRelativePath(const std::string& absolutePath,
                                              const std::string& path) {
	    const std::filesystem::path absolute{ absolutePath };
	    const std::filesystem::path relative{ path };
        return std::filesystem::relative(relative, absolute).generic_string();
	}

    bool FFileManager::isValidPath(const std::string& path) {
        std::ifstream test(path);
        if(!test.is_open()) {
            return false;
        }

        return true;
    }

    std::string FFileManager::getFilenameFromPath(const std::string& path) {
        return path.substr(path.find_last_of("/\\") + 1);
	}


}