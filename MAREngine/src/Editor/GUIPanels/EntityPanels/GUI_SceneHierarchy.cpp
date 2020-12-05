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


#include "GUI_SceneHierarchy.h"
#include "GUI_EntityCollectionPanel.h"
#include "GUI_EntityPanel.h"
#include "../GUI_Events.h"
#include "../GUI_TextEditor.h"

#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/ecs/SceneEvents.h"

#include "../../../Window/Window.h"

#include "../../EditorLogging.h"


namespace mar::editor {


	void GUI_SceneHierarchy::update(ecs::SceneManager* manager) {
		ImGui::Begin("Scene Hierarchy");

		buttonsAtPanel(manager);

		ImGui::Text("SCENE - %s", manager->getScene()->getName().c_str());
		ImGui::Separator();

		const auto& entities = manager->getScene()->getEntities();

		auto userSelectedEntity = [](const ecs::Entity& entity) {
			return ImGui::MenuItem(entity.getComponent<ecs::TagComponent>().tag.c_str());
		};

		const auto itEntity = std::find_if(entities.cbegin(), entities.cend(), userSelectedEntity);
		if (itEntity != entities.cend()) {
			GUI_Events::Instance()->onEntitySelected(*itEntity);
		}

		const auto& collections = manager->getScene()->getCollections();

		auto userSelectedCollection = [](const ecs::EntityCollection& collection) {
			return ImGui::MenuItem(collection.getComponent<ecs::TagComponent>().tag.c_str());
		};

		const auto itCollection = std::find_if(collections.cbegin(), collections.cend(), userSelectedCollection);
		if (itCollection != collections.cend()) {
			GUI_Events::Instance()->onEntityCollectionSelected(*itCollection);
		}

		popUpMenu(manager);
		
		ImGui::End();

		EDITOR_TRACE("GUI: scene_hierarchy");
	}

	void GUI_SceneHierarchy::buttonsAtPanel(ecs::SceneManager* manager) {
		if (ImGui::Button("+ E")) {
			GUI_Events::Instance()->onEntityCreated(manager);
		}

		ImGui::SameLine();

		if (ImGui::Button("+ EC")) {
			GUI_Events::Instance()->onEntityCollectionCreated(manager);
		}

		const auto& collection = GUI_EntityCollectionPanel::Instance()->getCurrentCollection();
		const bool collectionExists = &collection != nullptr;

		const auto& entity = GUI_EntityPanel::Instance()->getCurrentEntity();
		const bool entityExists = &entity != nullptr;

		if (collectionExists) {
			ImGui::SameLine();

			if (ImGui::Button("- EC")) {
				GUI_Events::Instance()->onEntityCollectionDeleted(manager, collection);
			}

			if(ImGui::Button("Copy - EC")) {
				GUI_Events::Instance()->onEntityCollectionCopied(manager, collection);
			}

			if (ImGui::Button("+ E in EC")) {
				GUI_Events::Instance()->onEntityCreatedAtCollection(collection);
			}

			if (entityExists) {
				ImGui::SameLine();

				if (ImGui::Button("- E in EC")) {
					GUI_Events::Instance()->onEntityDeletedFromCollection(collection, entity);
				}
			}
		}
		else if (entityExists) {
			ImGui::SameLine();

			if (ImGui::Button("Copy - E")) {
				GUI_Events::Instance()->onEntityCopied(manager, entity);
			}

			ImGui::SameLine();

			if (ImGui::Button("- E")) {
				GUI_Events::Instance()->onEntityDeleted(manager, entity);
			}
		}
	}

	void GUI_SceneHierarchy::popUpMenu(ecs::SceneManager* manager) {
		if (manager->isPlayMode()) {
			EDITOR_TRACE("GUI: return from scene_hierarchy_popup (PLAY MODE)");
			return;
		}

		if (ImGui::IsWindowFocused()) {
			if (window::Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
			}
		}

		// ACTUAL POP UP

		if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
			if (ImGui::MenuItem("Add EntityCollection to scene")) {
				GUI_Events::Instance()->onEntityCollectionCreated(manager);
			}

			if (ImGui::MenuItem("Add Entity to scene")) {
				GUI_Events::Instance()->onEntityCreated(manager);
			}

			const auto& collection = GUI_EntityCollectionPanel::Instance()->getCurrentCollection();
			const bool collectionExists = &collection != nullptr;

			const auto& entity = GUI_EntityPanel::Instance()->getCurrentEntity();
			const bool entityExists = &entity != nullptr;

			if (collectionExists) {
				const char* collection_tag = collection.getComponent<ecs::TagComponent>().tag.c_str();
				if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
					GUI_Events::Instance()->onEntityCreatedAtCollection(collection);
				}

				if (ImGui::MenuItem("Delete selected collection from Scene", collection_tag)) {
					GUI_Events::Instance()->onEntityCollectionDeleted(manager, collection);
				}
				
				if (entityExists) {
					const std::string delete_message = "Delete entity " + entity.getComponent<ecs::TagComponent>().tag + " from selected collection";
					if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
						GUI_Events::Instance()->onEntityDeletedFromCollection(collection, entity);
					}
				}
			}
			else if (entityExists) {

				const char* entity_tag = entity.getComponent<ecs::TagComponent>().tag.c_str();
				if (ImGui::MenuItem("Delete Selected Entity from Scene", entity_tag)) {
					GUI_Events::Instance()->onEntityDeleted(manager, entity);
				}
				
			}
	
			ImGui::EndPopup();
		}

		EDITOR_TRACE("GUI: scene_hierarchy_popup");
	}

	
}
