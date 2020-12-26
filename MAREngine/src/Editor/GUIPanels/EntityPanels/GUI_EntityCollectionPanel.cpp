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
#include "CommonComponentHandler.h"
#include "../GUI_TextEditor.h"
#include "../../EditorLogging.h"

#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/events/SceneEvents.h"

#include "../../../Window/Window.h"


namespace mar::editor {


	GUI_EntityCollectionPanel* GUI_EntityCollectionPanel::s_instance{ nullptr };


	void GUI_EntityCollectionPanel::initialize() {
		if (m_initialized) { return; }

		s_instance = this;
		m_initialized = true;
	}

	void GUI_EntityCollectionPanel::reset() {
		currentCollection = nullptr;
	}

	void GUI_EntityCollectionPanel::setCurrentCollection(const ecs::EntityCollection& collection) {
		currentCollection = &collection; 
	}

	const ecs::EntityCollection& GUI_EntityCollectionPanel::getCurrentCollection() const {
		return *currentCollection;
	}

	void GUI_EntityCollectionPanel::update() const {
		ImGui::Begin("EntityCollection Panel");

		if (!currentCollection) {
			ImGui::Text("No collection selected!");
			ImGui::End();
			return;
		}

		auto& tag = currentCollection->getComponent<ecs::TagComponent>();
		if (ImGui::CollapsingHeader("TagComponent")) {
			CommonComponentHandler::handleTagComponent(tag);
		}

		if (ImGui::CollapsingHeader("TransformComponent")) {
			handleTransformComponent();
		}

		if (currentCollection->hasComponent<ecs::ScriptComponent>() && ImGui::CollapsingHeader("ScriptComponent")) {
			CommonComponentHandler::handleScriptComponent(*currentCollection);
		}

		ImGui::Separator();

		const auto& entities = currentCollection->getEntities();

		auto userSelectedEntity = [](const ecs::Entity& entity) {
			return ImGui::MenuItem(entity.getComponent<ecs::TagComponent>().tag.c_str());
		};

		const auto itEntity = std::find_if(entities.cbegin(), entities.cend(), userSelectedEntity);
		if (itEntity != entities.cend()) {
			const auto& entity{ *itEntity };
			GUI_EntityPanel::Instance()->setCurrentEntity(entity);
		}

		popUpMenu(tag.tag.c_str());

		ImGui::End();
	}

	void GUI_EntityCollectionPanel::handleTransformComponent() const {
		auto& tran = currentCollection->getComponent<ecs::TransformComponent>();

		{ // Sliders
			bool updatedTransform = false;

			const auto lastCenter = tran.center;
			const auto lastAngles = tran.angles;
			const auto lastScale = tran.scale;
			if (ImGui::DragFloat3("Position", &tran.center.x, 0.05f, -200.f, 200.f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Rotation", &tran.angles.x, 0.5f, -360.f, 360.f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Scale", &tran.scale.x, 0.01f, 0.f, 20.0f, "%.2f", 1.f)) { updatedTransform = true; }

			if (updatedTransform) {
				const auto diffCenter = lastCenter != tran.center ? tran.center - lastCenter : maths::vec3();
				const auto diffAngles = lastAngles != tran.angles ? tran.angles - lastAngles : maths::vec3();
				const auto diffScale =	lastScale != tran.scale ? tran.scale - lastScale : maths::vec3();
				const ecs::TransformComponent diffTransform{ diffCenter, diffAngles, diffScale };

				ecs::SceneEvents::Instance().onCollectionTransformUpdate(currentCollection, diffTransform);
			}
		}

		{ // Reset to entities 
			if (ImGui::Button("Reset all entities to collection transform")) {
				ecs::SceneEvents::Instance().onCollectionTransformReset(currentCollection);
			}
		}
	}

	void GUI_EntityCollectionPanel::popUpMenu(const char* collection_tag) const {
		if (ImGui::IsWindowFocused()) {
			if (window::Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("EntityCollectionPopUp");
			}
		}

		if (ImGui::BeginPopup("EntityCollectionPopUp")) {
			const auto& entity = GUI_EntityPanel::Instance()->getCurrentEntity();
			const bool selectedEntityExists = &entity != nullptr;

			if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
				GUI_EntityPanel::Instance()->setCurrentEntity(currentCollection->createEntity());
			}

			if (!currentCollection->hasComponent<ecs::ScriptComponent>()) {
				if (ImGui::MenuItem("Add ScriptComponent")) {
					currentCollection->addComponent<ecs::ScriptComponent>();
					ecs::SceneEvents::Instance().onScriptAdd();
				}
			}

			if (selectedEntityExists) {
				const std::string delete_message = "Delete entity " + entity.getComponent<ecs::TagComponent>().tag + " from selected collection";
				if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
					currentCollection->destroyEntity(GUI_EntityPanel::Instance()->getCurrentEntity());
					GUI_EntityPanel::Instance()->reset();
					GUI_TextEditor::Instance()->reset();
					ecs::SceneEvents::Instance().onEntityRemove();
				}
			}
		
			ImGui::EndPopup();
		}
	}


}
