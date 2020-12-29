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


#include "SceneHierarchyWidget.h"
#include "EntityCollectionWidgetPanel.h"
#include "EntityWidgetPanel.h"
#include "../../WidgetEvents/EntityWidgetEvents.h"
#include "../../WidgetEvents/EntityCollectionWidgetEvents.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/events/SceneEvents.h"
#include "../../../Window/Window.h"
#include "../../EditorLogging.h"


namespace marengine {


	void WSceneHierarchyWidget::updateFrame() {
		ImGui::Begin("Scene Hierarchy");

		buttonsAtPanel();

		ImGui::Text("SCENE - %s", SceneManager::Instance->getScene()->getName().c_str());
		ImGui::Separator();

		const auto& entities = SceneManager::Instance->getScene()->getEntities();

		auto userSelectedEntity = [](const Entity& entity) {
			return ImGui::MenuItem(entity.getComponent<TagComponent>().tag.c_str());
		};

		const auto itEntity = std::find_if(entities.cbegin(), entities.cend(), userSelectedEntity);
		if (itEntity != entities.cend()) {
			FEntityWidgetEvents::onSelectedEntity(*itEntity);
		}

		const auto& collections{ SceneManager::Instance->getScene()->getCollections() };

		auto userSelectedCollection = [](const EntityCollection& collection) {
			return ImGui::MenuItem(collection.getComponent<TagComponent>().tag.c_str());
		};

		const auto itCollection = std::find_if(collections.cbegin(), collections.cend(), userSelectedCollection);
		if (itCollection != collections.cend()) {
			FEntityCollectionWidgetEvents::onSelectedCollection(*itCollection);
		}

		popUpMenu();
		
		ImGui::End();

		EDITOR_TRACE("GUI: scene_hierarchy");
	}

	void WSceneHierarchyWidget::buttonsAtPanel() {
		if (ImGui::Button("+ E")) {
			FEntityWidgetEvents::onCreateEntity();
		}

		ImGui::SameLine();

		if (ImGui::Button("+ EC")) {
			FEntityCollectionWidgetEvents::onCreateCollection();
		}

		const auto& collection = WEntityCollectionWidgetPanel::Instance->getCurrentCollection();
		const bool collectionExists = &collection != nullptr;

		const auto& entity = WEntityWidgetPanel::Instance->getCurrentEntity();
		const bool entityExists = &entity != nullptr;

		if (collectionExists) {
			ImGui::SameLine();

			if (ImGui::Button("- EC")) {
				FEntityCollectionWidgetEvents::onDestroyCollection(collection);
			}

			if(ImGui::Button("Copy - EC")) {
				FEntityCollectionWidgetEvents::onCopyCollection(collection);
			}

			if (ImGui::Button("+ E in EC")) {
				FEntityCollectionWidgetEvents::onEntityAddedToCollection(collection);
			}

			if (entityExists) {
				ImGui::SameLine();

				if (ImGui::Button("- E in EC")) {
					FEntityCollectionWidgetEvents::onEntityRemovedFromCollection(collection, entity);
				}
			}
		}
		else if (entityExists) {
			ImGui::SameLine();

			if (ImGui::Button("Copy - E")) {
				FEntityWidgetEvents::onCopyEntity(entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("- E")) {
				FEntityWidgetEvents::onDestroyEntity(entity);
			}
		}
	}

	void WSceneHierarchyWidget::popUpMenu() {
		if (SceneManager::Instance->isEditorMode() && ImGui::IsWindowFocused()) {
			if (Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
			}
		}

		// ACTUAL POP UP

		if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
			if (ImGui::MenuItem("Add EntityCollection to scene")) {
				FEntityCollectionWidgetEvents::onCreateCollection();
			}

			if (ImGui::MenuItem("Add Entity to scene")) {
				FEntityWidgetEvents::onCreateEntity();
			}

			const auto& collection = WEntityCollectionWidgetPanel::Instance->getCurrentCollection();
			const bool collectionExists = &collection != nullptr;

			const auto& entity = WEntityWidgetPanel::Instance->getCurrentEntity();
			const bool entityExists = &entity != nullptr;

			if (collectionExists) {
				const char* collection_tag = collection.getComponent<TagComponent>().tag.c_str();
				if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
					FEntityCollectionWidgetEvents::onEntityAddedToCollection(collection);
				}

				if (ImGui::MenuItem("Delete selected collection from Scene", collection_tag)) {
					FEntityCollectionWidgetEvents::onDestroyCollection(collection);
				}
				
				if (entityExists) {
					const std::string delete_message = "Delete entity " + entity.getComponent<TagComponent>().tag + " from selected collection";
					if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
						FEntityCollectionWidgetEvents::onEntityRemovedFromCollection(collection, entity);
					}
				}
			}
			else if (entityExists) {

				const char* entity_tag = entity.getComponent<TagComponent>().tag.c_str();
				if (ImGui::MenuItem("Delete Selected Entity from Scene", entity_tag)) {
					FEntityWidgetEvents::onDestroyEntity(entity);
				}
				
			}
	
			ImGui::EndPopup();
		}

		EDITOR_TRACE("GUI: scene_hierarchy_popup");
	}

	
}
