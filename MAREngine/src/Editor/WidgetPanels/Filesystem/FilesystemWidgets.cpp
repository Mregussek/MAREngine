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
#include "../ScriptEditor/ScriptIDEWidget.h"
#include "../EntityPanels/EntityWidgetPanel.h"
#include "../EntityPanels/EntityCollectionWidgetPanel.h"

#include "../../EditorLogging.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/events/SceneEvents.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/graphics/Mesh/MeshCreator.h"
#include "../../../Core/filesystem/EditorFilesystem.h"
#include "../../../Core/filesystem/SceneSerializer.h"
#include "../../../Core/filesystem/SceneDeserializer.h"
#include "../../../Engine.h"


namespace marengine {


	WFilesystemWidgets* WFilesystemWidgets::Instance{ nullptr };


	void WFilesystemWidgets::create() {
		Instance = this;
	}

	void WFilesystemWidgets::updateFrame() {
		displayNewSceneWindow();
		displaySaveSceneWindow(SceneManager::Instance->getScene());
		displayLoadSceneWindow();
		displayLoadOBJWindow(SceneManager::Instance->getScene());
	}

	void WFilesystemWidgets::openNewSceneWindow() const {
		ImGui::OpenPopup(m_nameNewScene);
	}

	void WFilesystemWidgets::openSaveSceneWindow() const {
		ImGui::OpenPopup(m_nameSaveScene);
	}

	void WFilesystemWidgets::openLoadSceneWindow() const {
		ImGui::OpenPopup(m_nameOpenScene);
	}

	void WFilesystemWidgets::openLoadOBJWindow() const {
		ImGui::OpenPopup(m_nameLoadOBJ);
	}

	void WFilesystemWidgets::openAssigningScriptWindow() const {
		ImGui::OpenPopup(m_nameAssignScript);
	}

	void WFilesystemWidgets::displayNewSceneWindow() {
		if (m_fileDialog.showFileDialog(m_nameNewScene, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1200, 800), ".marscene")) {		
			Scene* newScene = Scene::createEmptyScene(m_fileDialog.selected_fn);
			Filesystem::saveToFile(newScene, m_fileDialog.selected_path.c_str());
			delete newScene;

			MAREngine::Instance()->setLoadPath(m_fileDialog.selected_path);

			WEntityCollectionWidgetPanel::Instance->reset();
			WEntityWidgetPanel::Instance->reset();
			WScriptIDE::Instance->reset();

			MAREngine::Instance()->setRestart();
		}
	}

	void WFilesystemWidgets::displaySaveSceneWindow(Scene* scene) {
		if (m_fileDialog.showFileDialog(m_nameSaveScene, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1200, 800), ".marscene.json")) {
			//Filesystem::saveToFile(scene, m_fileDialog.selected_path.c_str());
			FSceneSerializer::saveSceneToFile(m_fileDialog.selected_path.c_str(), scene);
		}
	}

	void WFilesystemWidgets::displayLoadSceneWindow() {
		if (m_fileDialog.showFileDialog(m_nameOpenScene, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".marscene")) {
			MAREngine::Instance()->setLoadPath(m_fileDialog.selected_path);

			WEntityCollectionWidgetPanel::Instance->reset();
			WEntityWidgetPanel::Instance->reset();
			WScriptIDE::Instance->reset();

			MAREngine::Instance()->setRestart();
		}
	}

	void WFilesystemWidgets::displayLoadOBJWindow(Scene* scene) {
		if (m_fileDialog.showFileDialog(m_nameLoadOBJ, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".obj")) {
			const auto& collection = scene->createCollection();
			SceneEvents::Instance().onOBJload(collection, m_fileDialog.selected_fn, m_fileDialog.selected_path);
		}
	}

	void WFilesystemWidgets::displayAssigningScriptWindow(ScriptComponent& script) {
		if (m_fileDialog.showFileDialog(m_nameAssignScript, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".py")) {
			const auto& assetsPath = MAREngine::Instance()->getAssetsPath();

			script.script = m_fileDialog.selected_path;
			eraseSubstring(script.script, assetsPath);
		}
	}

	void WFilesystemWidgets::eraseSubstring(std::string& str, const std::string& toErase) {
		size_t pos = std::string::npos;

		while ((pos = str.find(toErase)) != std::string::npos) {
			str.erase(pos, toErase.length());
		}
	}


}
