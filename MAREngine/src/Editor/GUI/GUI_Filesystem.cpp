/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "GUI_Filesystem.h"

#include "GUI_TextEditor.h"
#include "GUI_EntityPanel.h"
#include "GUI_EntityCollectionPanel.h"


namespace mar {
	namespace editor {


		imgui_addons::ImGuiFileBrowser GUI_Filesystem::s_fileDialog;


		void GUI_Filesystem::Filesystem_NewScene(const char* name) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1000, 510), ".marscene,.mrsc")) {
				GUI_EntityCollectionPanel::reset();
				GUI_EntityPanel::reset();
				GUI_TextEditor::Instance().reset();
				
				engine::MAREngine::getEngine()->setLoadPath("BrandNewScene");
				engine::MAREngine::getEngine()->setRestart();
			}

			EDITOR_TRACE("GUI: Filesystem_NewScene");
		}

		void GUI_Filesystem::Filesystem_SaveScene(const char* name, ecs::Scene* scene_to_save) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::SAVE, ImVec2(1000, 510), ".marscene,.mrsc")) {
				if (s_fileDialog.selected_path.find(".marscene") != std::string::npos) {
					Filesystem::saveToFile(scene_to_save, s_fileDialog.selected_path.c_str());
				}
				else if (s_fileDialog.selected_path.find(".mrsc") != std::string::npos) {
					Filesystem::saveToFile(scene_to_save, s_fileDialog.selected_path.c_str());
				}
				else {
					EDITOR_ERROR("GUI_FILESYSTEM: given path {} has no correct extension, expected ones are .marscene and .mrsc!", s_fileDialog.selected_path)
				}
			}
		}

		void GUI_Filesystem::Filesystem_LoadScene(const char* name) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1000, 510), ".marscene,.mrsc")) {
				engine::MAREngine::getEngine()->setLoadPath(s_fileDialog.selected_path);

				GUI_EntityCollectionPanel::reset();
				GUI_EntityPanel::reset();
				GUI_TextEditor::Instance().reset();

				engine::MAREngine::getEngine()->setRestart();
			}
		}

		void GUI_Filesystem::Filesystem_LoadOBJfile(const char* name, ecs::Scene* scene) {
			if (s_fileDialog.showFileDialog(name, imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, ImVec2(1000, 510), ".obj")) {
				auto& collection = scene->createCollection();
				auto& tag = collection.getComponent<ecs::TagComponent>();
				tag.tag = s_fileDialog.selected_fn;
				
				graphics::MeshCreator::loadOBJ(s_fileDialog.selected_fn, s_fileDialog.selected_path, collection);
				ecs::SceneEvents::Instance().onCollectionOBJloaded(collection);
			}
		}



} }