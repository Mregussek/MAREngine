/**
 *				MAREngine - open source 3D game engine
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


#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H


#include "../../mar.h"
#include "GUI_EntityPanel.h"
#include "GUI_TextEditor.h"
#include "../../Core/ecs/ECS/EntityCollection.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/Components.h"


namespace mar {
	namespace editor {


		struct GUI_EntityCollectionPanel {
			static ecs::EntityCollection* currentCollection;
			static int32_t currentIndex;

			static void Scene_EntityCollection_Modify() {
				ImGui::Begin("EntityCollection Panel");

				if (!currentCollection) {
					ImGui::Text("No collection selected!");
					ImGui::End();
					return;
				}
				
				auto& tag = currentCollection->getComponent<ecs::TagComponent>();

				ImGui::Text("ENTITYCOLLECTION --- %s", tag.tag.c_str());
				ImGui::Separator();

				static char* input;
				input = (char*)tag.tag.c_str();

				if (ImGui::InputText("Tag", input, 25))
					tag.tag = std::string(input);

				ImGui::Separator();

				auto& entities = currentCollection->getEntities();

				for (int32_t i = 0; i < (int32_t)entities.size(); i++) {
					if (ImGui::MenuItem(entities[i].getComponent<ecs::TagComponent>().tag.c_str())) {
						GUI_EntityPanel::currentEntity = &currentCollection->getEntity(i);
						GUI_EntityPanel::currentIndex = i;
					}
				}

				Scene_EntityCollection_PopUp(tag.tag.c_str());

				ImGui::End();
			}

			static void reset() {
				currentCollection = nullptr;
				currentIndex = -1;
			}

		private:

			static void Scene_EntityCollection_PopUp(const char* collection_tag) {
				static bool b = false;

				if (ImGui::IsWindowFocused())
					b = window::Input::isMousePressed(MAR_MOUSE_BUTTON_2);
				else
					b = false;

				if (b) {
					ImGui::OpenPopup("EntityCollectionPopUp");
					if (window::Input::isMousePressed(MAR_MOUSE_BUTTON_1))
						b = false;
				}

				if (ImGui::BeginPopup("EntityCollectionPopUp")) {
					if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
						GUI_EntityPanel::currentEntity = &GUI_EntityCollectionPanel::currentCollection->createEntity();
						GUI_EntityPanel::currentIndex = GUI_EntityCollectionPanel::currentCollection->getEntities().size() - 1;
					}

					if (GUI_EntityPanel::currentEntity) {
						std::string delete_message = "Delete entity " + GUI_EntityPanel::currentEntity->getComponent<ecs::TagComponent>().tag + " from selected collection";
						if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
							currentCollection->destroyEntity(GUI_EntityPanel::currentIndex);
							GUI_EntityPanel::reset();
							GUI_TextEditor::Instance().reset();
							ecs::SceneEvents::Instance().onEntityRemove();
						}
					}

					ImGui::EndPopup();
				}
			}

		};


} }



#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H
