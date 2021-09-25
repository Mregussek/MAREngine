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


#include "../public/FileManager.h"
#include "../../../Logging/Logger.h"


namespace marengine {


    static std::string removePatternFromString(std::string s, const std::string& p);


	void FFileManager::loadFile(std::string& stringToFill, const char* path) {
        // TODO: implement it
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Loading {} file...", path);

		FILE* file = fopen(path, "rb");
		fseek(file, 0, SEEK_END);
		const long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		stringToFill.resize(length);
		const size_t rc = fread(stringToFill.data(), 1, length, file);
		fclose(file);

        if (rc == length) {
            MARLOG_INFO(ELoggerType::FILESYSTEM, "Loaded {} file...", path);
        }
        else {
            MARLOG_WARN(ELoggerType::FILESYSTEM, "During {} loading some issue occured, maybe EOF?", path);
        }
	}

	void FFileManager::saveAsFile(const std::string& sourceCode, const char* path) {
        // TODO: implement it
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Saving {} file...", path);
        MARLOG_WARN(ELoggerType::FILESYSTEM, "Saving is not implement!");
	}

    bool FFileManager::isContainingExtension(const std::string& path, const std::string& extension) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Checking, if {} contains extension {}...", path, extension);

        const std::string currentExtension{ path.substr(path.find_last_of('.') + 1) };
        if(currentExtension == extension) {
            MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Path {} contains {} extension", path, extension);
            return true;
        }

        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Path {} does not contain {} extension", path, extension);
        return false;
    }

    std::string FFileManager::getAbsolutePath(const std::string& relativePath) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Retrieving absolute path from {} relative one...", relativePath);
        std::string absolute{ std::filesystem::absolute(relativePath).u8string() };
        std::replace(absolute.begin(), absolute.end(), '\\', '/');
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Returning absolute path {} ...", absolute);
        return absolute;
	}

    std::string FFileManager::getRelativePath(const std::string& absolutePath,
                                              const std::string& path) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Retrieving relative path {} from {} absolute one...", path, absolutePath);
	    const std::filesystem::path absolute{ absolutePath };
	    const std::filesystem::path relative{ path };
        std::string result{ std::filesystem::relative(relative, absolute).generic_string() };
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Returning relative path {} ... ", result);
        return result;
	}

    bool FFileManager::isValidPath(const std::string& path) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Validating, if given string is a path: {} ...", path);
        std::ifstream test(path);
        if(!test.is_open()) {
            MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Given string is a path: {}", path);
            return false;
        }

        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Given string is NOT a path: {}", path);
        return true;
    }

    std::string FFileManager::getFilenameFromPath(const std::string& path) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Retrieving filename from path: {} ...", path);
        std::string filename{ path.substr(path.find_last_of("/\\") + 1) };
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Retrieved filename: {}", filename);
        return filename;
	}

    std::string FFileManager::getCurrentExePath() {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Retrieving current executable path...");
        char buffer[MAX_PATH];
	    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	    const std::string::size_type pos{ std::string(buffer).find_last_of("\\/") };
        std::string currentExePath{ std::string(buffer).substr(0, pos) };
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Returning current exe path: {}", currentExePath);
	    return currentExePath;
    }

    std::string FFileManager::deleteFilenameFromPath(std::string path) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Deleting filename from path: {} ...", path);
        path = removePatternFromString(path, getFilenameFromPath(path));
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Returning path with deleted filename from it: {}", path);
        return path;
    }

    std::string FFileManager::joinPaths(const std::string& path1, const std::string& path2) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Going to join paths: {} + {} ...", path1, path2);
        const std::filesystem::path joinedPath{ std::filesystem::path(path1) / std::filesystem::path(path2)};
        std::string result{ joinedPath.generic_string() };
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Joined paths: {}", result);
        return result;
    }

    bool FFileManager::isPathEndingWithSubstring(const std::string& path, const std::string& substring) {
        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Checking if path {} is ending with substring {}", path, substring);
        const char* pPath{ path.c_str() };
        const char* pSubstring{ substring.c_str() };
        const auto pathLength{ path.size() };
        const auto substringLength{ substring.size() };
        const bool result{ substringLength <= pathLength && !strcmp(pPath + pathLength - substringLength, pSubstring) };
        MARLOG_DEBUG(ELoggerType::FILESYSTEM, "Result of substring {} end at path {}: {}", substring, path, result);
        return result;
    }



    std::string removePatternFromString(std::string s, const std::string& p) {
        const std::string::size_type n{ p.length() };
        for (std::string::size_type i = s.find(p); i != std::string::npos; i = s.find(p)) {
            s.erase(i, n);
        }
        return s;
    }


}