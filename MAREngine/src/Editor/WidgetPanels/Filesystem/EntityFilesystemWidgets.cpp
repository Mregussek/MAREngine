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


#include "EntityFilesystemWidgets.h"
#include "FilesystemWidgets.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../../../Engine.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Components/Components.h"


namespace marengine {


	WEntityFilesystemWidgets* WEntityFilesystemWidgets::Instance{ nullptr };

	void WEntityFilesystemWidgets::create() {
		Instance = this;
	}

	void WEntityFilesystemWidgets::updateFrame() {
		constexpr auto loadOBJcallback = [](const std::string& path, const std::string& filename) {

		};

		constexpr auto assignScriptCallback = [](const std::string& path, const std::string& filename) {
			const auto& assetsPath{ MAREngine::Instance()->getAssetsPath() };
			auto& scriptComponent{ WEntityWidgetPanel::Instance->getCurrentEntity().getComponent<ScriptComponent>() };

			scriptComponent.script = path;

			// Erase assets path substring from loaded path
			size_t pos = std::string::npos;
			while ((pos = scriptComponent.script.find(assetsPath)) != std::string::npos) {
				scriptComponent.script.erase(pos, assetsPath.length());
			}
		};

		WFilesystemWidgets::Instance->displayOpenWidget(m_loadOBJname, m_objExtension, loadOBJcallback);
		WFilesystemWidgets::Instance->displayOpenWidget(m_assignScriptName, m_pythonExtension, assignScriptCallback);
	}

	void WEntityFilesystemWidgets::openLoadOBJWidget() const {
		WFilesystemWidgets::Instance->openWidget(m_loadOBJname.c_str());
	}

	void WEntityFilesystemWidgets::openAssignPythonScriptWidget() const {
		WFilesystemWidgets::Instance->openWidget(m_assignScriptName.c_str());
	}


}