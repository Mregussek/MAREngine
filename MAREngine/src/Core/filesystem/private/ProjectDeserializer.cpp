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


namespace marengine {


    void FFileDeserializer::loadProjectFromFile(FProject* pProject, const std::string& path) {
        MARLOG_INFO(ELoggerType::FILESYSTEM, "Loading project project... -> {}", path);

        using namespace projectjson;
        if (!FFileManager::isContainingExtension(path, "cfg")) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} does not point to project file!", path);
            return;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} cannot be opened!", path);
            return;
        }

        nlohmann::json json{ nlohmann::json::parse(file) };
        file.close();

        pProject->setProjectName(json[jProject][jProjectName]);
        pProject->setProjectPath(json[jProject][jProjectPath]);
        pProject->setSceneStartup(json[jProject][jProjectSceneStartup]);
        pProject->setProjectVersion(json[jProject][jProjectVersion]);

        uint32 i = 0;
        for(nlohmann::json& jsonScene : json[jScenes]) {
            Scene* pScene{ pProject->addScene(json[jScenes][i][jScenesName]) };
            const std::string sceneFilename{ json[jScenes][i][jScenesPath] };
            const std::string pathToScene{ pProject->getScenesPath() + sceneFilename };
            loadSceneFromFile(pScene, pathToScene);
            i++;
        }

        MARLOG_INFO(ELoggerType::FILESYSTEM, "Loaded Project -> {}", path);
    }


}
