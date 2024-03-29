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
        MARLOG_TRACE(ELoggerType::NORMAL, "Checking, if engine config exists...");
        if(FFileManager::isValidPath(m_configPath)) {
            MARLOG_DEBUG(ELoggerType::NORMAL, "EngineConfig does exist...");
            return true;
        }

        MARLOG_DEBUG(ELoggerType::NORMAL, "EngineConfig does NOT exist...");
        return false;
    }

    void FEngineConfig::load() {
        MARLOG_TRACE(ELoggerType::NORMAL, "Loading {} file", m_configPath);
        FFileDeserializer::loadConfigFromFile(this, m_configPath);
        MARLOG_DEBUG(ELoggerType::NORMAL, "Loaded {} file", m_configPath);
    }

    void FEngineConfig::save() const {
        MARLOG_TRACE(ELoggerType::NORMAL, "Saving {} file", m_configPath);
        FFileSerializer::saveConfigToFile(this, m_configPath);
        MARLOG_DEBUG(ELoggerType::NORMAL, "Saved {} file", m_configPath);
    }

    FMinimalProjectInfo* FEngineConfig::addProjectInfo() {
        MARLOG_TRACE(ELoggerType::NORMAL, "Adding new minimal project info...");
        return &m_existingProjects.emplace_back();
    }

    const FMinimalProjectInfo* FEngineConfig::getProjectInfo(const std::string& projectName) const {
        MARLOG_TRACE(ELoggerType::NORMAL, "Looking for MinimalProjectInfo with ProjectName: {} ...", projectName);
        auto projectExists = [&projectName](const FMinimalProjectInfo& projectInfo){
            return projectInfo.projectName == projectName;
        };
        const auto it =
                std::find_if(m_existingProjects.cbegin(), m_existingProjects.cend(), projectExists);

        if(it != m_existingProjects.cend()) {
            const FMinimalProjectInfo* pMinimalProjectInfo{ &(*it) };
            MARLOG_INFO(ELoggerType::NORMAL, "Found MinimalProjectInfo:\nName: {}\nPath: {}",
                pMinimalProjectInfo->projectName, pMinimalProjectInfo->projectPath);
            return pMinimalProjectInfo;
        }

        MARLOG_WARN(ELoggerType::NORMAL, "Could not find project at configuration -> {}", projectName);
        return nullptr;
    }

    void FEngineConfig::createDefault() const {
        MARLOG_TRACE(ELoggerType::NORMAL, "Creating default MAREngine .cfg file...")
        FEngineConfig tmp;
        FMinimalProjectInfo* pProjectInfo{ tmp.addProjectInfo() };
        pProjectInfo->projectName = "DefaultProject";
        pProjectInfo->projectPath = FFileManager::joinPaths(FFileManager::getCurrentExePath(), pProjectInfo->projectName);
        FFileSerializer::saveConfigToFile(&tmp, m_configPath);
        MARLOG_INFO(ELoggerType::NORMAL, "Created default MAREngine .cfg file at {} with project:\nName: {}\nPath: {}", 
            m_configPath, pProjectInfo->projectName, pProjectInfo->projectPath);
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

