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
#include "MARJsonDefinitions.inl"
#include "../../../Logging/Logger.h"
#include "../../../ProjectManager.h"
#include "../../../EngineConfig.h"


namespace marengine {


    void FFileSerializer::saveConfigToFile(const FEngineConfig* pEngineConfig,
                                           const std::string& path) {
        using namespace configjson;
        
        std::ofstream ss(path, std::ios::out | std::ios::trunc);
        if (!ss.is_open()) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Cannot save EngineConfig to -> {}", path);
            return;
        }

        nlohmann::json json;

        const FEngineInfo& engineInfo{ pEngineConfig->getEngineInfo() };
        json[jEngineInfo][jEngineVersion] = engineInfo.version;
        json[jEngineInfo][jEngineName] = engineInfo.name;
        json[jEngineInfo][jEngineAuthors] = engineInfo.authors;

        const FEngineEditorSettings& editorSettings{ pEngineConfig->getEditorSettings() };
        json[jEditorConfig][jEditorTheme] = (int32)editorSettings.theme;

        const FEngineWindowSettings& windowSettings{ pEngineConfig->getWindowSettings() };
        json[jWindowConfig][jWindowVerticalSync] = windowSettings.verticalSync;

        const auto& existingProjects{ pEngineConfig->getProjectInfos() };
        const uint32 projectsCount{ existingProjects.size() };
        for(uint32 i = 0; i < projectsCount; i++) {
            json[jMinimalProjects][i][jProjectName] = existingProjects.at(i).projectName;
            json[jMinimalProjects][i][jProjectPath] = existingProjects.at(i).projectPath;
        }

        ss << json.dump(4);
		ss.close();

		MARLOG_INFO(ELoggerType::FILESYSTEM, "Saved EngineConfig to -> {}", path);
    }


}
