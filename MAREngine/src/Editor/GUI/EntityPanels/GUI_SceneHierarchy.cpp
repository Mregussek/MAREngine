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
#include "../GUI_TextEditor.h"

#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Entity/EntityCollection.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneEvents.h"
#include "../../../Core/ecs/SceneManager.h"

#include "../../../Window/Input.h"
#include "../../../Core/graphics/Renderer/RenderPipeline.h"

#include "../../EditorLogging.h"


namespace mar {
	namespace editor {


		void GUI_SceneHierarchy::Scene_Hierarchy(ecs::SceneManager* manager) {
			ImGui::Begin("Scene Hierarchy");

			ImGui::Text("SCENE - %s", manager->getScene()->getName().c_str());
			ImGui::Separator();

			auto& entities = manager->getScene()->getEntities();

			ImGui::Text("ENTITIES --- %d", entities.size());
			ImGui::Separator();

			for (int32_t i = 0; i < (int32_t)entities.size(); i++) {
				if (ImGui::MenuItem(entities[i].getComponent<ecs::TagComponent>().tag.c_str())) {
					GUI_EntityCollectionPanel::reset();
					GUI_EntityPanel::currentEntity = &manager->getScene()->getEntity(i);
					GUI_EntityPanel::currentIndex = i;
				}
			}

			ImGui::Separator();

			auto& collections = manager->getScene()->getCollections();

			ImGui::Text("Collections --- %d", collections.size());
			ImGui::Separator();

			for (int32_t i = 0; i < (int32_t)collections.size(); i++) {
				if (ImGui::MenuItem(collections[i].getComponent<ecs::TagComponent>().tag.c_str())) {
					GUI_EntityPanel::reset();
					GUI_EntityCollectionPanel::currentCollection = &manager->getScene()->getCollection(i);
					GUI_EntityCollectionPanel::currentIndex = i;
				}
			}

			Scene_Hierarchy_PopUp(manager);
			Scene_Statistics(manager);
			
			ImGui::End();

			EDITOR_TRACE("GUI: scene_hierarchy");
		}

		void GUI_SceneHierarchy::Scene_Hierarchy_PopUp(ecs::SceneManager* manager) {
			if (manager->isPlayMode()) {
				EDITOR_TRACE("GUI: return from scene_hierarchy_popup (PLAY MODE)");
				return;
			}

			// SHOULD POP BE OPEN? 

			static bool b = false;

			if (ImGui::IsWindowFocused()) b = window::Input::isMousePressed(MAR_MOUSE_BUTTON_2);
			else b = false;

			if (b) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
				if (window::Input::isMousePressed(MAR_MOUSE_BUTTON_1)) b = false;
			}

			// ACTUAL POP UP

			if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
				if (ImGui::MenuItem("Add EntityCollection to scene")) {
					GUI_EntityPanel::reset();
					GUI_EntityCollectionPanel::currentCollection = &manager->getScene()->createCollection();
					GUI_EntityCollectionPanel::currentIndex = manager->getScene()->getCollections().size() - 1;
				}

				if (ImGui::MenuItem("Add Entity to scene")) {
					GUI_EntityCollectionPanel::reset();
					GUI_EntityPanel::currentEntity = &manager->getScene()->createEntity();
					GUI_EntityPanel::currentIndex = manager->getScene()->getEntities().size() - 1;
				}

				if (GUI_EntityCollectionPanel::currentCollection) {
					const char* collection_tag = GUI_EntityCollectionPanel::currentCollection->getComponent<ecs::TagComponent>().tag.c_str();

					if (ImGui::MenuItem("Add Entity to selected collection", collection_tag)) {
						GUI_EntityPanel::currentEntity = &GUI_EntityCollectionPanel::currentCollection->createEntity();
						GUI_EntityPanel::currentIndex = GUI_EntityCollectionPanel::currentCollection->getEntities().size() - 1;
					}

					if (ImGui::MenuItem("Delete selected collection from Scene", collection_tag)) {
						manager->getScene()->destroyCollection(GUI_EntityCollectionPanel::currentIndex);
						GUI_EntityCollectionPanel::reset();
						GUI_EntityPanel::reset();
						ecs::SceneEvents::Instance().onCollectionRemove();
					}

					if (GUI_EntityPanel::currentEntity) {
						std::string delete_message = "Delete entity " + GUI_EntityPanel::currentEntity->getComponent<ecs::TagComponent>().tag + " from selected collection";
						if (ImGui::MenuItem(delete_message.c_str(), collection_tag)) {
							manager->getScene()->destroyEntityAtCollection(GUI_EntityCollectionPanel::currentIndex, GUI_EntityPanel::currentIndex);
							GUI_EntityPanel::reset();
							ecs::SceneEvents::Instance().onEntityRemove();
						}
					}
				}
				else if (GUI_EntityPanel::currentEntity) {
					const char* entity_tag = GUI_EntityPanel::currentEntity->getComponent<ecs::TagComponent>().tag.c_str();
					if (ImGui::MenuItem("Delete Selected Entity from Scene", entity_tag)) {
						manager->getScene()->destroyEntity(GUI_EntityPanel::currentIndex);
						GUI_EntityPanel::reset();
						ecs::SceneEvents::Instance().onEntityRemove();
					}
				}
					
				ImGui::EndPopup();
			}

			EDITOR_TRACE("GUI: scene_hierarchy_popup");
		}

		void GUI_SceneHierarchy::Scene_Statistics(ecs::SceneManager* manager) {
			ImGui::Begin("Statistics Menu");

			auto& render_pip = graphics::RenderPipeline::getInstance();
			auto& stats = render_pip.getStatistics();

			stats.shapesCount += render_pip.getTransforms().size();
			stats.verticesCount += render_pip.getVertices().size();
			stats.indicesCount += render_pip.getIndicesCount();
			stats.trianglesCount += render_pip.getIndicesCount() / 3;
			stats.entitiesCount += manager->getScene()->getEntities().size();
			stats.entityCollectionsCount += manager->getScene()->getCollections().size();

			stats.allEntitiesCount += stats.entitiesCount;
			for (auto& collection : manager->getScene()->getCollections()) {
				stats.allEntitiesCount += collection.getEntities().size();
			}

			ImGui::Text("Draw Calls: %d", stats.drawCallsCount);
			ImGui::Text("Vertices: %d" , stats.verticesCount);
			ImGui::Text("Indices: %d", stats.indicesCount);
			ImGui::Text("Triangles: %d", stats.trianglesCount);
			ImGui::Text("Entities: %d", stats.entitiesCount);
			ImGui::Text("EntityCollections: %d", stats.entityCollectionsCount);
			ImGui::Text("All Entities: %d", stats.allEntitiesCount);

			ImGui::Separator();

			static double lasttime = GetTickCount() * 0.001;
			static double currenttime;
			static double fps = 0.0;
			static int frames = 0;

			currenttime = GetTickCount64() * 0.001;
			frames++;

			if (currenttime - lasttime > 1.0) {
				fps = frames / (currenttime - lasttime);
				frames = 0;
				lasttime = currenttime;
			}

			ImGui::Text("My FPS: %f ms/frame", fps);

			ImGui::Separator();

			ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
			ImGui::Text("ms/frame: %f", 1000.0f / ImGui::GetIO().Framerate);

			ImGui::End();

			EDITOR_TRACE("GUI: scene_statistics");
		}

	
} }