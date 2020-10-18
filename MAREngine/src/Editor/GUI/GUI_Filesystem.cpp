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
#include "../../Core/ecs/SceneEvents.h"
#include "../../Core/ecs/Entity/EntityCollection.h"

#include "../../Core/graphics/Mesh/MeshCreator.h"

#include "../Filesystem/EditorFilesystem.h"
#include "../../Engine.h"


namespace mar::editor {


	void GUI_Filesystem::SetOpenNewScene() {
		const auto& scenesPath = engine::MAREngine::getEngine()->getScenesPath();
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", m_nameNewScene, ".marscene", scenesPath);
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
	}

	void GUI_Filesystem::Filesystem_NewScene() {
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(m_nameNewScene)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {



			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(m_nameNewScene);
		}
	}

	void GUI_Filesystem::SetOpenSaveScene() {
		const auto& scenesPath = engine::MAREngine::getEngine()->getScenesPath();
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", m_nameSaveScene, ".marscene", scenesPath);
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
	}

	void GUI_Filesystem::Filesystem_SaveScene(ecs::Scene* scene_to_save) {
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(m_nameSaveScene)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

				const std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
				Filesystem::saveToFile(scene_to_save, filePathName.c_str());

			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(m_nameSaveScene);
		}
	}

	void GUI_Filesystem::SetOpenLoadScene() {
		const auto& scenesPath = engine::MAREngine::getEngine()->getScenesPath();
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", m_nameOpenScene, ".marscene", scenesPath);
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".marscene", ImVec4(0, 1, 0, 0.9f));
	}

	void GUI_Filesystem::Filesystem_LoadScene() {
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(m_nameOpenScene)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

				engine::MAREngine::getEngine()->setLoadPath(igfd::ImGuiFileDialog::Instance()->GetFilePathName());

				GUI_EntityCollectionPanel::Instance()->reset();
				GUI_EntityPanel::Instance()->reset();
				GUI_TextEditor::Instance().reset();

				engine::MAREngine::getEngine()->setRestart();

			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(m_nameOpenScene);
		}
	}

	void GUI_Filesystem::SetOpenLoadOBJfile() {
		const auto& assetsPath = engine::MAREngine::getEngine()->getAssetsPath();
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", m_nameLoadOBJ, ".obj", assetsPath);
		igfd::ImGuiFileDialog::Instance()->SetExtentionInfos(".obj", ImVec4(0, 1, 0, 0.9f));
	}

	void GUI_Filesystem::Filesystem_LoadOBJfile(ecs::Scene* scene) {
		if (igfd::ImGuiFileDialog::Instance()->FileDialog(m_nameLoadOBJ)) {
			if (igfd::ImGuiFileDialog::Instance()->IsOk == true) {

				const std::string filePathName = igfd::ImGuiFileDialog::Instance()->GetFilePathName();
				const std::string filename = igfd::ImGuiFileDialog::Instance()->GetCurrentFileName();

				const auto& collection = scene->createCollection();
				auto& tag = collection.getComponent<ecs::TagComponent>();
				auto& crc = collection.addComponent<ecs::CollectionRenderableComponent>();

				tag.tag = filename;
				crc = filePathName;

				graphics::MeshCreator::loadOBJ(filename, filePathName, collection);
				ecs::SceneEvents::Instance().onCollectionOBJloaded(collection);

			}

			igfd::ImGuiFileDialog::Instance()->CloseDialog(m_nameLoadOBJ);
		}	
	}


}
