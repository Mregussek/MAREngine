/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H
#define MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H


#include "../../mar.h"
#include "GUI_EntityPanel.h"
#include "GUI_TextEditor.h"
#include "../../Core/ecs/ECS/EntityCollection.h"
#include "../../Core/ecs/ECS/Entity.h"
#include "../../Core/ecs/ECS/ComponentsEntity.h"


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
				
				auto& tag = currentCollection->getComponent<ecs::CollectionTagComponent>();

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

				Scene_EntityCollection_PopUp();

				ImGui::End();
			}

			static void reset() {
				currentCollection = nullptr;
				currentIndex = -1;
			}

		private:

			static void Scene_EntityCollection_PopUp() {
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
					if (ImGui::MenuItem("Add Entity to selected Collection")) {
						GUI_EntityPanel::currentEntity = &currentCollection->createEntity();
						GUI_EntityPanel::currentIndex = currentCollection->getEntities().size() - 1;
					}

					if (GUI_EntityPanel::currentEntity)
						if (ImGui::MenuItem("Delete Selected Entity from selected collection")) {
							currentCollection->destroyEntity(GUI_EntityPanel::currentIndex);
							GUI_EntityPanel::reset();
							GUI_TextEditor::Instance().reset();
							ecs::SceneEvents::Instance().onEntityRemove();
						}

					ImGui::EndPopup();
				}
			}

		};


} }



#endif // !MAR_ENGINE_EDITOR_GUI_ENTITY_COLLECTION_PANEL_H
