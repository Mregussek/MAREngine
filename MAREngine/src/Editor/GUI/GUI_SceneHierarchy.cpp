/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI_SceneHierarchy.h"

#include "GUI_EntityCollectionPanel.h"
#include "GUI_EntityPanel.h"
#include "GUI_TextEditor.h"

#include "../../Core/graphics/Renderer/RendererBatch.h"
#include "../EditorLogging.h"

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
				if (ImGui::MenuItem(collections[i].getComponent<ecs::CollectionTagComponent>().tag.c_str())) {
					GUI_EntityCollectionPanel::currentCollection = &manager->getScene()->getCollection(i);
					GUI_EntityCollectionPanel::currentIndex = i;
				}
			}

			Scene_Hierarchy_PopUp(manager);

			ImGui::End();

			EDITOR_TRACE("GUI: scene_hierarchy");
		}

		void GUI_SceneHierarchy::Scene_Hierarchy_PopUp(ecs::SceneManager* manager) {
			if (manager->isPlayMode()) {
				EDITOR_TRACE("GUI: return from scene_hierarchy_popup (PLAY MODE)");
				return;
			}

			static bool b = false;

			if (ImGui::IsWindowFocused())
				b = window::Input::isMousePressed(MAR_MOUSE_BUTTON_2);
			else
				b = false;

			if (b) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
				if (window::Input::isMousePressed(MAR_MOUSE_BUTTON_1))
					b = false;
			}

			if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
				if (ImGui::MenuItem("Add EntityCollection to scene")) {
					GUI_EntityCollectionPanel::currentCollection = &manager->getScene()->createCollection();
					GUI_EntityCollectionPanel::currentIndex = manager->getScene()->getCollections().size() - 1;
				}

				if (ImGui::MenuItem("Add Entity to scene")) {
					GUI_EntityCollectionPanel::reset();
					GUI_EntityPanel::currentEntity = &manager->getScene()->createEntity();
					GUI_EntityPanel::currentIndex = manager->getScene()->getEntities().size() - 1;
				}

				if (GUI_EntityCollectionPanel::currentCollection) {
					if (ImGui::MenuItem("Add Entity to selected Collection")) {
						GUI_EntityPanel::currentEntity = &GUI_EntityCollectionPanel::currentCollection->createEntity();
						GUI_EntityPanel::currentIndex = GUI_EntityCollectionPanel::currentCollection->getEntities().size() - 1;
					}

					if (ImGui::MenuItem("Deleted Selected EntityCollection from Scene")) {
						manager->getScene()->destroyCollection(GUI_EntityCollectionPanel::currentIndex);
						GUI_EntityCollectionPanel::reset();
						GUI_EntityPanel::reset();
						GUI_TextEditor::Instance().reset();
						ecs::SceneEvents::Instance().onCollectionRemove();
					}

					if (GUI_EntityPanel::currentEntity)
						if (ImGui::MenuItem("Delete Selected Entity from selected collection")) {
							manager->getScene()->destroyEntityAtCollection(GUI_EntityCollectionPanel::currentIndex, GUI_EntityPanel::currentIndex);
							GUI_EntityPanel::reset();
							GUI_TextEditor::Instance().reset();
							ecs::SceneEvents::Instance().onEntityRemove();
						}
				}	
				else if (GUI_EntityPanel::currentEntity)
					if (ImGui::MenuItem("Delete Selected Entity from Scene")) {
						manager->getScene()->destroyEntity(GUI_EntityPanel::currentIndex);
						GUI_EntityPanel::reset();
						GUI_TextEditor::Instance().reset();
						ecs::SceneEvents::Instance().onEntityRemove();
					}

				ImGui::EndPopup();
			}

			EDITOR_TRACE("GUI: scene_hierarchy_popup");
		}

		void GUI_SceneHierarchy::Scene_Statistics() {
			ImGui::Begin("Statistics Menu");

			auto& stats = graphics::RenderPipeline::getInstance().getStatistics();

			ImGui::Text("Draw Calls: %d", stats.drawCallsCount);
			ImGui::Text("Vertices: %d" , stats.verticesCount);
			ImGui::Text("Indices: %d", stats.indicesCount);
			ImGui::Text("Triangles: %d", stats.trianglesCount);

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