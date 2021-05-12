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


#include "MAREngine.h"
#include "Logging/Logger.h"
#include "Core/filesystem/public/FileManager.h"


namespace marengine {


    bool FEngineConfig::exists() const {
        if(FFileManager::isValidPath(m_configPath)) {
            return true;
        }
        else {
            return false;
        }
    }

    void FEngineConfig::load() {
        FFileDeserializer::loadConfigFromFile(this, m_configPath);
    }

    void FEngineConfig::save() const {
        FFileSerializer::saveConfigToFile(this, m_configPath);
    }

    FMinimalProjectInfo* FEngineConfig::addProjectInfo() {
        return &m_existingProjects.emplace_back();
    }

    const FMinimalProjectInfo* FEngineConfig::getProjectInfo(const std::string& projectName) const {
        auto projectExists = [&projectName](const FMinimalProjectInfo& projectInfo){
            return projectInfo.projectName == projectName;
        };
        const auto it =
                std::find_if(m_existingProjects.cbegin(), m_existingProjects.cend(), projectExists);
        if(it != m_existingProjects.cend()) {
            return &(*it);
        }

        MARLOG_ERR(ELoggerType::NORMAL, "Could not find project at configuration -> {}", projectName);
        return nullptr;
    }

    void FEngineConfig::createDefault() const {
        FEngineConfig tmp;
        FFileSerializer::saveConfigToFile(&tmp, m_configPath);
    }

    const FEngineInfo& FEngineConfig::getEngineInfo() const {
        return m_engineInfo;
    }

    const FEngineEditorSettings& FEngineConfig::getEditorSettings() const {
        return m_editorSettings;
    }

    const FEngineWindowSettings& FEngineConfig::getWindowSettings() const {
        return m_windowSettings;
    }

    const std::vector<FMinimalProjectInfo>& FEngineConfig::getProjectInfos() const {
        return m_existingProjects;
    }

    void FEngineConfig::setEngineInfo(const FEngineInfo& engineInfo) {
        m_engineInfo = engineInfo;
    }

    void FEngineConfig::setEditorSettings(const FEngineEditorSettings& editorSettings) {
        m_editorSettings = editorSettings;
    }

    void FEngineConfig::setWindowSettings(const FEngineWindowSettings& windowSettings) {
        m_windowSettings = windowSettings;
    }

}

