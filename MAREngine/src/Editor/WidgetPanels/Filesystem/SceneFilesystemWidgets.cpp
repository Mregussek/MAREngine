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


#include "SceneFilesystemWidgets.h"
#include "FilesystemWidgets.h"
#include "../ScriptEditor/ScriptIDEWidget.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../EntityPanels/EntityCollectionWidgetPanel.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/filesystem/SceneSerializer.h"
#include "../../../Engine.h"


namespace marengine {


	WSceneFilesystemWidgets* WSceneFilesystemWidgets::Instance{ nullptr };

	void WSceneFilesystemWidgets::create() {
		Instance = this;
	}

	void WSceneFilesystemWidgets::updateFrame() {
		constexpr auto newSceneCallback = [](const std::string& path, const std::string& filename) {
			Scene* newScene = Scene::createEmptyScene(filename);
			FSceneSerializer::saveSceneToFile(path.c_str(), newScene);
			delete newScene;

			MAREngine::Instance()->setLoadPath(path);

			WEntityCollectionWidgetPanel::Instance->reset();
			WEntityWidgetPanel::Instance->reset();
			WScriptIDE::Instance->reset();

			MAREngine::Instance()->setRestart();
		};

		constexpr auto saveSceneCallback = [](const std::string& path, const std::string& filename) {
			FSceneSerializer::saveSceneToFile(path.c_str(), SceneManager::Instance->getScene());
		};

		constexpr auto loadSceneCallback = [](const std::string& path, const std::string& filename) {
			MAREngine::Instance()->setLoadPath(path);

			WEntityCollectionWidgetPanel::Instance->reset();
			WEntityWidgetPanel::Instance->reset();
			WScriptIDE::Instance->reset();

			MAREngine::Instance()->setRestart();
		};

		WFilesystemWidgets::Instance->displaySaveWidget(m_newSceneName, extensions, newSceneCallback);
		WFilesystemWidgets::Instance->displaySaveWidget(m_saveSceneName, extensions, saveSceneCallback);
		WFilesystemWidgets::Instance->displayOpenWidget(m_loadSceneName, extensions, loadSceneCallback);
	}

	void WSceneFilesystemWidgets::openNewSceneWidget() const {
		WFilesystemWidgets::Instance->openWidget(m_newSceneName.c_str());
	}

	void WSceneFilesystemWidgets::openSaveSceneWidget() const {
		WFilesystemWidgets::Instance->openWidget(m_saveSceneName.c_str());
	}

	void WSceneFilesystemWidgets::openLoadSceneWidget() const {
		WFilesystemWidgets::Instance->openWidget(m_loadSceneName.c_str());
	}


}