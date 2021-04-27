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


#ifndef MARENGINE_ENGINECONFIG_H
#define MARENGINE_ENGINECONFIG_H


#include "mar.h"
#include "ProjectManager.h"


namespace marengine {


    struct FEngineInfo {
        const std::string version{ "v0.0.1" };
        const std::string name{ "MAREngine" };
        const std::string authors{ "Mateusz Rzeczyca" };
    };

    enum class EEngineEditorTheme {
        DEFAULT, DARK_THEME
    };

    struct FEngineEditorSettings {
        EEngineEditorTheme theme{ EEngineEditorTheme::DEFAULT };
    };

    struct FEngineWindowSettings {
        uint8 verticalSync{ 1 };
    };


    class FEngineConfig {
    public:

        void createDefault() const;

        bool exists() const;
        void load();
        void save() const;

        const FEngineInfo& getEngineInfo() const;
        const FEngineEditorSettings& getEditorSettings() const;
        const FEngineWindowSettings& getWindowSettings() const;
        const FMinimalProjectInfo* getProjectInfo(const std::string& projectName) const;

        FMinimalProjectInfo* addProjectInfo();

    private:

        std::string m_configPath{ "marengine.cfg" };
        std::vector<FMinimalProjectInfo> m_existingProjects;
        FEngineInfo m_engineInfo;
        FEngineEditorSettings m_editorSettings;
        FEngineWindowSettings m_windowSettings;

    };


}


#endif //MARENGINE_ENGINECONFIG_H
