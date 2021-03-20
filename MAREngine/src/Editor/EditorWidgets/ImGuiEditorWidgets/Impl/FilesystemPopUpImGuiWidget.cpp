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


#include "FilesystemPopUpImGuiWidget.h"


namespace marengine {


    void FFilesystemPopUpImGuiWidget::openWidget(const std::string& widgetName) const {
        ImGui::OpenPopup(widgetName.c_str());
    }

    void FFilesystemPopUpImGuiWidget::displaySaveWidget(const std::string &widgetName, const std::string &extensions,
                                                        CallbackFunc callback) {
        displayWidget(widgetName, extensions, DialogMode::SAVE, callback);
    }

    void FFilesystemPopUpImGuiWidget::displayOpenWidget(const std::string &widgetName, const std::string &extensions,
                                                        CallbackFunc callback) {
        displayWidget(widgetName, extensions, DialogMode::OPEN, callback);
    }

    void FFilesystemPopUpImGuiWidget::displayWidget(const std::string &widgetName, const std::string &extensions,
                                                    DialogMode dialogMode, CallbackFunc callback) {
        const ImVec2 windowSize{ 1200, 800 };
        const bool userSelectedFile{ m_fileDialog.showFileDialog(widgetName, dialogMode, windowSize, extensions) };
        if (userSelectedFile) {
            callback(m_fileDialog.selected_path, m_fileDialog.selected_fn);
        }
    }


}
