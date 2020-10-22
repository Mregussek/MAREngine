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
#include "../GUI_TextEditor.h"
#include "../../EditorLogging.h"

#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/SceneEvents.h"

#include "../../../Window/Input.h"


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

		ImGui::Text("ENTITYCOLLECTION --- %s", tag.tag.c_str());
		ImGui::Separator();

		handleTagComponent(tag);
		
		ImGui::Separator();

		handleTransformComponent();

		ImGui::Separator();

		const auto& entities = currentCollection->getEntities();

		auto userSelectedEntity = [](const ecs::Entity& entity) {
			return ImGui::MenuItem(entity.getComponent<ecs::TagComponent>().tag.c_str());
		};

		const auto itEntity = std::find_if(entities.cbegin(), entities.cend(), userSelectedEntity);
		if (itEntity != entities.cend()) {
			const auto& entity = *itEntity;
			GUI_EntityPanel::Instance()->setCurrentEntity(entity);
		}

		popUpMenu(tag.tag.c_str());

		ImGui::End();
	}

	void GUI_EntityCollectionPanel::handleTagComponent(ecs::TagComponent& tag) const {
		constexpr size_t inputSize = 50;
		static char collectionName[inputSize]{ "" };

		std::fill(std::begin(collectionName), std::end(collectionName), '\0');
		std::copy(tag.tag.begin(), tag.tag.end(), collectionName);

		if (ImGui::InputText(" - tag", collectionName, inputSize)) {
			tag.tag = std::string(collectionName);
		}
	}

	void GUI_EntityCollectionPanel::handleTransformComponent() const {
		auto& tran = currentCollection->getComponent<ecs::TransformComponent>();

		{ // Sliders
			bool updatedTransform = false;

			static maths::vec3 lastCenter;
			static maths::vec3 lastAngles;
			static maths::vec3 lastScale;
			static float lastGeneral;

			lastCenter = tran.center;
			lastAngles = tran.angles;
			lastScale = tran.scale;
			lastGeneral = tran.general_scale;

			if (ImGui::DragFloat3("Position", &tran.center.x, 0.05f, -200.0f, 200.0f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Rotation", &tran.angles.x, 0.5f, -360.f, 360.f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Scale", &tran.scale.x, 0.01f, 0.f, 20.0f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat("GeneralScale", &tran.general_scale, 0.01f, 0.001f, 10.f, "%.3f", 1.f)) { updatedTransform = true; }

			if (lastGeneral != tran.general_scale) { tran.scale += tran.general_scale - lastGeneral; }

			if (ImGui::Button("Reset to default scale")) {
				tran.general_scale = 1.f;
				tran.scale.x = 1.f;
				tran.scale.y = 1.f;
				tran.scale.z = 1.f;
				updatedTransform = true;
			}

			if (updatedTransform) {
				const auto diffCenter = lastCenter != tran.center ? tran.center - lastCenter : maths::vec3();
				const auto diffAngles = lastAngles != tran.angles ? tran.angles - lastAngles : maths::vec3();
				const auto diffScale =	lastScale != tran.scale ? tran.scale - lastScale : maths::vec3();
				const auto diffGeneralScale = lastGeneral != tran.general_scale ? tran.general_scale - lastGeneral : 0.f;

				auto createRelativeTransformToCollection = [&diffCenter, &diffAngles, &diffScale, diffGeneralScale](const ecs::Entity& entity) {
					auto& entityTransform = entity.getComponent<ecs::TransformComponent>();

					entityTransform.center += diffCenter;
					entityTransform.angles += diffAngles;
					entityTransform.scale += diffScale;
					entityTransform.general_scale += diffGeneralScale;

					entityTransform.recalculate();
				};

				const auto& entities = currentCollection->getEntities();
				std::for_each(entities.cbegin(), entities.cend(), createRelativeTransformToCollection);

				ecs::SceneEvents::Instance().onCollectionTransformUpdate();
			}
		}

		{ // Reset to entities 
			if (ImGui::Button("Reset all entities to collection transform")) {
				const auto& entities = currentCollection->getEntities();
				std::for_each(entities.begin(), entities.end(), [&collectionTransform = std::as_const(tran)](const ecs::Entity& entity) {
					auto& transform = entity.getComponent<ecs::TransformComponent>();

					transform.center = collectionTransform.center;
					transform.angles = collectionTransform.angles;
					transform.scale = collectionTransform.scale;
					transform.general_scale = collectionTransform.general_scale;

					transform.recalculate();
				});

				ecs::SceneEvents::Instance().onCollectionTransformUpdate();
			}
		}
	}

	void GUI_EntityCollectionPanel::popUpMenu(const char* collection_tag) const {
		if (ImGui::IsWindowFocused()) {
			if (window::Input::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("EntityCollectionPopUp");
			}
		}

		if (ImGui::BeginPopup("EntityCollectionPopUp")) {
			const auto& entity = GUI_EntityPanel::Instance()->getCurrentEntity();
			const bool selectedEntityExists = &entity != nullptr;

			if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
				GUI_EntityPanel::Instance()->setCurrentEntity(currentCollection->createEntity());
			}

			/*
			if (selectedEntityExists) {
				std::string delete_message = "Delete entity " + entity.getComponent<ecs::TagComponent>().tag + " from selected collection";
				if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
					currentCollection->destroyEntity(GUI_EntityPanel::currentIndex);
					GUI_EntityPanel::Instance()->reset();
					GUI_TextEditor::Instance()->reset();
					ecs::SceneEvents::Instance().onEntityRemove();
				}
			}
			*/
			ImGui::EndPopup();
		}
	}


}
