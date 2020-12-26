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


#include "GUI_Filesystem.h"
#include "GUI_TextEditor.h"
#include "EntityPanels/GUI_EntityPanel.h"
#include "EntityPanels/GUI_EntityCollectionPanel.h"

#include "../EditorLogging.h"

#include "../../Core/ecs/Scene.h"
#include "../../Core/events/SceneEvents.h"
#include "../../Core/ecs/Entity/EntityCollection.h"

#include "../../Core/graphics/Mesh/MeshCreator.h"

#include "../Filesystem/EditorFilesystem.h"
#include "../../Engine.h"


namespace marengine {


	GUI_Filesystem* GUI_Filesystem::s_instance{ nullptr };


	void GUI_Filesystem::initialize() {
		s_instance = this;
	}

	void GUI_Filesystem::openNewSceneWindow() {
		ImGui::OpenPopup(m_nameNewScene);
	}

	void GUI_Filesystem::displayNewSceneWindow() {
		if (m_fileDialog.showFileDialog(m_nameNewScene, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1200, 800), ".marscene")) {		
			Scene* newScene = Scene::createEmptyScene(m_fileDialog.selected_fn);
			Filesystem::saveToFile(newScene, m_fileDialog.selected_path.c_str());
			delete newScene;

			MAREngine::Instance()->setLoadPath(m_fileDialog.selected_path);

			GUI_EntityCollectionPanel::Instance()->reset();
			GUI_EntityPanel::Instance()->reset();
			GUI_TextEditor::Instance()->reset();

			MAREngine::Instance()->setRestart();
		}
	}

	void GUI_Filesystem::openSaveSceneWindow() {
		ImGui::OpenPopup(m_nameSaveScene);
	}

	void GUI_Filesystem::displaySaveSceneWindow(Scene* scene) {
		if (m_fileDialog.showFileDialog(m_nameSaveScene, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1200, 800), ".marscene")) {
			Filesystem::saveToFile(scene, m_fileDialog.selected_path.c_str());
		}
	}

	void GUI_Filesystem::openLoadSceneWindow() {
		ImGui::OpenPopup(m_nameOpenScene);
	}

	void GUI_Filesystem::displayLoadSceneWindow() {
		if (m_fileDialog.showFileDialog(m_nameOpenScene, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".marscene")) {
			MAREngine::Instance()->setLoadPath(m_fileDialog.selected_path);

			GUI_EntityCollectionPanel::Instance()->reset();
			GUI_EntityPanel::Instance()->reset();
			GUI_TextEditor::Instance()->reset();

			MAREngine::Instance()->setRestart();
		}
	}

	void GUI_Filesystem::openLoadOBJWindow() {
		ImGui::OpenPopup(m_nameLoadOBJ);
	}

	void GUI_Filesystem::displayLoadOBJWindow(Scene* scene) {
		if (m_fileDialog.showFileDialog(m_nameLoadOBJ, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".obj")) {
			const auto& collection = scene->createCollection();
			SceneEvents::Instance().onOBJload(collection, m_fileDialog.selected_fn, m_fileDialog.selected_path);
		}
	}

	void GUI_Filesystem::openAssigningScriptWindow() {
		ImGui::OpenPopup(m_nameAssignScript);
	}

	void GUI_Filesystem::displayAssigningScriptWindow(ScriptComponent& script) {
		if (m_fileDialog.showFileDialog(m_nameAssignScript, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1200, 800), ".py")) {
			const auto& assetsPath = MAREngine::Instance()->getAssetsPath();

			script.script = m_fileDialog.selected_path;
			eraseSubstring(script.script, assetsPath);
		}
	}

	void GUI_Filesystem::eraseSubstring(std::string& str, const std::string& toErase) {
		size_t pos = std::string::npos;

		while ((pos = str.find(toErase)) != std::string::npos) {
			str.erase(pos, toErase.length());
		}
	}


}
