/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#include "GUI_Filesystem.h"


namespace mar {
	namespace editor {


		void GUI_Filesystem::Filesystem_SaveScene(bool& should_be_opened, ecs::Scene* scene_to_save) {
			ImGui::Begin("Save File");

			static char filename[30]{ "empty" };

			ImGui::InputText(".marscene", filename, 30);

			ImGui::Separator();

			static std::string save;
			save = "resources/mar_files/" + std::string(filename) + ".marscene";

			ImGui::Text("Saving to: ");
			ImGui::SameLine();
			ImGui::Text(save.c_str());

			ImGui::Separator();

			if (ImGui::Button("Save to selected name"))
				Filesystem::saveToFile(scene_to_save, save.c_str());

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				should_be_opened = false;

			ImGui::End();

			EDITOR_TRACE("GUI: filesystem_savescene");
		}

		void GUI_Filesystem::Filesystem_LoadScene(bool& should_be_opened) {
			ImGui::Begin("Open File");

			ImGui::Text("Select file, which you want to be opened:");

			static char input[30];
			ImGui::InputText(".marscene", input, 30);

			ImGui::Separator();

			static std::string will_open;
			will_open = "resources/mar_files/" + std::string(input) + ".marscene";

			ImGui::Text("File, which is going to be opened:");
			ImGui::SameLine();
			ImGui::Text(will_open.c_str());

			ImGui::Text("PLEASE MAKE SURE PATH IS CORRECT!");

			if (ImGui::Button("Open Selected File")) {
				engine::MAREngine::getEngine()->setLoadPath(will_open);
				engine::MAREngine::getEngine()->setRestart();
			}

			ImGui::SameLine();

			if (ImGui::Button("Close"))
				should_be_opened = false;

			ImGui::End();

			EDITOR_TRACE("GUI: filesystem_loadscene");
		}



} }