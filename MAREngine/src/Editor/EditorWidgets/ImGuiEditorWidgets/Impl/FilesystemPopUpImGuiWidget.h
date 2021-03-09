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


#ifndef MARENGINE_FILESYSTEMPOPUPIMGUIWIDGET_H
#define MARENGINE_FILESYSTEMPOPUPIMGUIWIDGET_H


#include "../../../../mar.h"


namespace marengine {


    class FFilesystemPopUpImGuiWidget {

        typedef imgui_addons::ImGuiFileBrowser::DialogMode DialogMode;
        typedef void (*CallbackFunc)(const std::string &path, const std::string &filename);

    public:

        void openWidget(const std::string& widgetName) const;

        void displaySaveWidget(const std::string& widgetName, const std::string& extensions, CallbackFunc callback);

        void displayOpenWidget(const std::string& widgetName, const std::string& extensions, CallbackFunc callback);

    private:

        void displayWidget(const std::string& widgetName, const std::string& extensions, DialogMode dialogMode,
                           CallbackFunc callback);


        imgui_addons::ImGuiFileBrowser m_fileDialog;

    };


}



#endif //MARENGINE_FILESYSTEMPOPUPIMGUIWIDGET_H
