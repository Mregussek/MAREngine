/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "GUI_SceneHierarchy.h"

#include "GUI_EntityManagement.h"
#include "GUI_TextEditor.h"

#include "../../Core/graphics/Renderer/RendererBatch.h"
#include "../EditorLogging.h"

namespace mar {
	namespace editor {


		void GUI_SceneHierarchy::Scene_Hierarchy(ecs::SceneManager* manager, ecs::Entity* entity, int32_t& index) {
			ImGui::Begin("Scene Hierarchy");

			ImGui::Text(" - ");
			ImGui::SameLine();
			ImGui::Text(manager->getScene()->getName().c_str());

			auto& entities = manager->getScene()->getEntities();

			for (int32_t i = 0; i < (int32_t)entities.size(); i++) {
				std::string& s = entities[i].getComponent<ecs::TagComponent>();
				if (ImGui::MenuItem(s.c_str())) {
					index = i;
					entity = &manager->getScene()->getEntity(index);
					GUI_EntityManagement::currentEntity = entity;
					GUI_EntityManagement::currentIndex = index;
				}
			}

			Scene_Hierarchy_PopUp(manager, entity, index);

			ImGui::End();

			EDITOR_TRACE("GUI: scene_hierarchy");
		}

		void GUI_SceneHierarchy::Scene_Hierarchy_PopUp(ecs::SceneManager* manager, ecs::Entity* entity, int32_t& index) {
			if (manager->isPlayMode()) {
				EDITOR_TRACE("GUI: return from scene_hierarchy_popup (PLAY MODE)");
				return;
			}

			static bool b = false;

			if (ImGui::IsWindowFocused())
				b = window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_2);
			else
				b = false;

			if (b) {
				ImGui::OpenPopup("SceneHierarchyPopUp");
				if (window::Input::isMousePressed_NotViewport(MAR_MOUSE_BUTTON_1))
					b = false;
			}

			if (ImGui::BeginPopup("SceneHierarchyPopUp")) {
				if (ImGui::MenuItem("Add Entity to scene")) {
					entity = &manager->getScene()->createEntity();
					index = manager->getScene()->getEntities().size() - 1;
					GUI_EntityManagement::currentEntity = entity;
					GUI_EntityManagement::currentIndex = index;
				}

				if (entity)
					if (ImGui::MenuItem("Delete Selected Entity from Scene")) {
						manager->getScene()->destroyEntity(index);
						index = -1;
						entity = nullptr;
						GUI_EntityManagement::currentEntity = entity;
						GUI_EntityManagement::currentIndex = index;
						GUI_TextEditor::Instance().setEditorText("def main():\n\tpass\n");
						GUI_TextEditor::Instance().setEditorTitle("Empty");
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
			ImGui::Text("Entities Count (with Renderable): %d", stats.shapesCount);
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