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


#include "FilesystemWidgets.h"
#include "FilesystemWidgetInfo.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/events/SceneEvents.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/graphics/Mesh/MeshCreator.h"
#include "../../../Core/filesystem/SceneSerializer.h"
#include "../../../Engine.h"


namespace marengine {


	WFilesystemWidgets* WFilesystemWidgets::Instance{ nullptr };


	void WFilesystemWidgets::create() {
		Instance = this;
	}

	void WFilesystemWidgets::openWidget(const std::string& widgetName) const {
		ImGui::OpenPopup(widgetName.c_str());
	}

	void WFilesystemWidgets::displaySaveWidget(const FFilesystemWidgetInfo& widgetInfo, void(*callback)(const std::string& path, const std::string& filename)) {
		if (m_fileDialog.showFileDialog(widgetInfo.name, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, widgetInfo.size, widgetInfo.extensions)) {
			callback(m_fileDialog.selected_path, m_fileDialog.selected_fn);
		}
	}

	void WFilesystemWidgets::displayOpenWidget(const FFilesystemWidgetInfo& widgetInfo, void(*callback)(const std::string& path, const std::string& filename)) {
		if (m_fileDialog.showFileDialog(widgetInfo.name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, widgetInfo.size, widgetInfo.extensions)) {
			callback(m_fileDialog.selected_path, m_fileDialog.selected_fn);
		}
	}


}
