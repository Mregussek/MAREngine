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


#include "EntityCollectionWidgetPanel.h"
#include "EntityWidgetPanel.h"
#include "CommonComponentHandler.h"
#include "../../WidgetEvents/EventsCollectionWidget.h"
#include "../../EditorLogging.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/events/SceneEvents.h"
#include "../../../Window/Window.h"


namespace marengine {


	WEntityCollectionWidgetPanel* WEntityCollectionWidgetPanel::Instance{ nullptr };


	void WEntityCollectionWidgetPanel::create() {
		Instance = this;
	}

	void WEntityCollectionWidgetPanel::reset() {
		currentCollection = nullptr;
	}

	void WEntityCollectionWidgetPanel::setCurrentCollection(const EntityCollection& collection) {
		currentCollection = &collection; 
	}

	const EntityCollection& WEntityCollectionWidgetPanel::getCurrentCollection() const {
		return *currentCollection;
	}

	void WEntityCollectionWidgetPanel::updateFrame() {
		ImGui::Begin("EntityCollection Panel");

		if (!currentCollection) {
			ImGui::Text("No collection selected!");
			ImGui::End();
			return;
		}

		auto& tag = currentCollection->getComponent<TagComponent>();
		if (ImGui::CollapsingHeader("TagComponent")) {
			CommonComponentHandler::handleTagComponent(tag);
		}

		if (ImGui::CollapsingHeader("TransformComponent")) {
			handleTransformComponent();
		}

		if (currentCollection->hasComponent<ScriptComponent>() && ImGui::CollapsingHeader("ScriptComponent")) {
			CommonComponentHandler::handleScriptComponent(*currentCollection);
		}

		ImGui::Separator();

		const auto& entities = currentCollection->getEntities();

		auto userSelectedEntity = [](const Entity& entity) {
			return ImGui::MenuItem(entity.getComponent<TagComponent>().tag.c_str());
		};

		const auto itEntity = std::find_if(entities.cbegin(), entities.cend(), userSelectedEntity);
		if (itEntity != entities.cend()) {
			FEventsCollectionWidget::onSelectedEntityFromCollection(*itEntity);
		}

		popUpMenu(tag.tag.c_str());

		ImGui::End();
	}

	void WEntityCollectionWidgetPanel::handleTransformComponent() const {
		auto& tran = currentCollection->getComponent<TransformComponent>();

		{ // Sliders
			bool updatedTransform = false;

			const auto lastCenter = tran.center;
			const auto lastAngles = tran.angles;
			const auto lastScale = tran.scale;

			if (CommonComponentHandler::drawVec3Control("Position", tran.center, 0.f, 100.f)) { updatedTransform = true; }
			if (CommonComponentHandler::drawVec3Control("Rotation", tran.angles, 0.f, 100.f)) { updatedTransform = true; }
			if (CommonComponentHandler::drawVec3Control("Scale", tran.scale, 0.f, 100.f)) { updatedTransform = true; }

			ImGui::NewLine();

			if (updatedTransform) {
				const auto diffCenter = lastCenter != tran.center ? tran.center - lastCenter : maths::vec3();
				const auto diffAngles = lastAngles != tran.angles ? tran.angles - lastAngles : maths::vec3();
				const auto diffScale =	lastScale != tran.scale ? tran.scale - lastScale : maths::vec3();
				const TransformComponent diffTransform{ diffCenter, diffAngles, diffScale };

				SceneEvents::Instance().onCollectionTransformUpdate(currentCollection, diffTransform);
			}
		}

		{ // Reset to entities 
			if (ImGui::Button("Reset all entities to collection transform")) {
				SceneEvents::Instance().onCollectionTransformReset(currentCollection);
			}
		}
	}

	void WEntityCollectionWidgetPanel::popUpMenu(const char* collection_tag) const {
		if (ImGui::IsWindowFocused()) {
			if (Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("EntityCollectionPopUp");
			}
		}

		if (ImGui::BeginPopup("EntityCollectionPopUp")) {
			const auto& entity = WEntityWidgetPanel::Instance->getCurrentEntity();
			const bool selectedEntityExists = &entity != nullptr;

			if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
				FEventsCollectionWidget::onEntityAddedToCollection(*currentCollection);
			}

			if (!currentCollection->hasComponent<ScriptComponent>()) {
				if (ImGui::MenuItem("Add ScriptComponent")) {
					currentCollection->addComponent<ScriptComponent>();
					SceneEvents::Instance().onScriptAdd();
				}
			}

			if (selectedEntityExists) {
				const std::string delete_message = "Delete entity " + entity.getComponent<TagComponent>().tag + " from selected collection";
				if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
					FEventsCollectionWidget::onEntityRemovedFromCollection(*currentCollection, entity);
				}
			}
		
			ImGui::EndPopup();
		}
	}


}
