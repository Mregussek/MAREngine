/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_W_FILESYSTEM_WIDGETS_H
#define MAR_ENGINE_W_FILESYSTEM_WIDGETS_H


#include "../../../mar.h"
#include "../IWidgetPanel.h"


namespace marengine {

	class Scene; 
	struct ScriptComponent;


	class WFilesystemWidgets : public IWidgetPanel {
	public:

		static WFilesystemWidgets* Instance;

		virtual void create() override;

		void openWidget(const char* widgetName) const;

		void displayOpenWidget(const std::string& name, const std::string& extensions, void(*callback)(const std::string& path, const std::string& filename));

		void displaySaveWidget(const std::string& name, const std::string& extensions, void(*callback)(const std::string& path, const std::string& filename));

	private:

		imgui_addons::ImGuiFileBrowser m_fileDialog;

	};


}


#endif // !MAR_ENGINE_W_FILESYSTEM_WIDGETS_H
