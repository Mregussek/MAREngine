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


#include "GUI_EntityCollectionPanel.h"
#include "GUI_EntityPanel.h"
#include "GUI_TextEditor.h"
#include "../EditorLogging.h"

#include "../../Core/ecs/Components/Components.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"
#include "../../Core/ecs/SceneEvents.h"

#include "../../Window/Input.h"


namespace mar {
	namespace editor {


		ecs::EntityCollection* GUI_EntityCollectionPanel::currentCollection{ nullptr };
		int32_t GUI_EntityCollectionPanel::currentIndex{ -1 };

		void GUI_EntityCollectionPanel::reset() {
			currentCollection = nullptr;
			currentIndex = -1;
		}

		void GUI_EntityCollectionPanel::Scene_EntityCollection_Modify() {
			ImGui::Begin("EntityCollection Panel");

			if (!currentCollection) {
				ImGui::Text("No collection selected!");
				ImGui::End();
				return;
			}

			auto& tag = currentCollection->getComponent<ecs::TagComponent>();

			ImGui::Text("ENTITYCOLLECTION --- %s", tag.tag.c_str());
			ImGui::Separator();

			Handle_TagComponent(tag);
			
			ImGui::Separator();

			Handle_TransformComponent();

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

		void GUI_EntityCollectionPanel::Handle_TagComponent(ecs::TagComponent& tag) {
			static char* input;
			input = (char*)tag.tag.c_str();

			if (ImGui::InputText("Tag", input, 25))
				tag.tag = std::string(input);
		}

		void GUI_EntityCollectionPanel::Handle_TransformComponent() {
			auto& tran = currentCollection->getComponent<ecs::TransformComponent>();

			// Sliders
			{
				static bool updated_transform;
				updated_transform = false;

				static maths::vec3 last_center;
				static maths::vec3 last_angles;
				static maths::vec3 last_scale;
				static float last_general;
				last_center = tran.center;
				last_angles = tran.angles;
				last_scale = tran.scale;
				last_general = tran.general_scale;

				if (ImGui::DragFloat3("Position", maths::vec3::value_ptr_nonconst(tran.center), 0.05f, -200.0f, 200.0f, "%.2f", 1.f)) updated_transform = true;
				if (ImGui::DragFloat3("Rotation", maths::vec3::value_ptr_nonconst(tran.angles), 0.5f, -360.f, 360.f, "%.2f", 1.f)) updated_transform = true;
				if (ImGui::DragFloat3("Scale", maths::vec3::value_ptr_nonconst(tran.scale), 0.01f, 0.f, 20.0f, "%.2f", 1.f)) updated_transform = true;
				if (ImGui::DragFloat("GeneralScale", &tran.general_scale, 0.01f, 0.001f, 10.f, "%.3f", 1.f)) updated_transform = true;

				if (last_general != tran.general_scale)
					tran.scale += tran.general_scale - last_general;

				if (ImGui::Button("Reset to default scale")) {
					tran.general_scale = 1.f;
					tran.scale.x = 1.f;
					tran.scale.y = 1.f;
					tran.scale.z = 1.f;
					updated_transform = true;
				}

				if (updated_transform) {
					maths::vec3 diff_center = last_center != tran.center ? tran.center - last_center : maths::vec3{ 0.f, 0.f, 0.f };
					maths::vec3 diff_angles = last_angles != tran.angles ? tran.angles - last_angles : maths::vec3{ 0.f, 0.f, 0.f };
					maths::vec3 diff_scale = last_scale != tran.scale ? tran.scale - last_scale : maths::vec3{ 0.f, 0.f, 0.f };
					float diff_generalscale = last_general != tran.general_scale ? tran.general_scale - last_general : 0.f;

					for (size_t i = 0; i < currentCollection->getEntitiesCount(); i++) {
						auto& entity = currentCollection->getEntity(i);
						auto& transform = entity.getComponent<ecs::TransformComponent>();

						transform.center += diff_center;
						transform.angles += diff_angles;
						transform.scale += diff_scale;
						transform.general_scale += diff_generalscale;

						transform.recalculate();
					}

					ecs::SceneEvents::Instance().onCollectionTransformUpdate();
				}
			}

			{ // Reset to entities 
				if (ImGui::Button("Reset all entities to collection transform")) {
					for (size_t i = 0; i < currentCollection->getEntitiesCount(); i++) {
						auto& entity = currentCollection->getEntity(i);
						auto& transform = entity.getComponent<ecs::TransformComponent>();

						transform.center = tran.center;
						transform.angles = tran.angles;
						transform.scale = tran.scale;
						transform.general_scale = tran.general_scale;

						transform.recalculate();
					}

					ecs::SceneEvents::Instance().onCollectionTransformUpdate();
				}
			}
		}

		void GUI_EntityCollectionPanel::Scene_EntityCollection_PopUp(const char* collection_tag) {
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


} }
