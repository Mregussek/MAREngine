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


#include "GUI_EntityPanel.h"
#include "../GUI_TextEditor.h"
#include "../GUI_Filesystem.h"

#include "../../EditorLogging.h"

#include "../../../Core/graphics/Mesh/MeshCreator.h"

#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/SceneEvents.h"

#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Engine.h"

#include "../../Filesystem/EditorFilesystem.h"
#include "../../../Window/Input.h"


namespace mar::editor {


	GUI_EntityPanel* GUI_EntityPanel::s_instance{ nullptr };


	void GUI_EntityPanel::initialize() {
		if (m_initialized) { return; }

		s_instance = this;
		m_initialized = true;
	}

	void GUI_EntityPanel::reset() {
		currentEntity = nullptr;
	}

	void GUI_EntityPanel::setRenderCam(graphics::RenderCamera* renderCam) {
		render_cam = renderCam;
	}

	void GUI_EntityPanel::setCurrentEntity(const ecs::Entity& entity) { 
		currentEntity = &entity; 
	}

	const ecs::Entity& GUI_EntityPanel::getCurrentEntity() const { 
		return *currentEntity;
	}

	void GUI_EntityPanel::update(bool isPlayMode) {
		ImGui::Begin("Entity Modification");

		if (!currentEntity) {
			ImGui::Text("No entity selected!");
			ImGui::End();
			return;
		}

		if (isPlayMode) {
			displayPlayMode(isPlayMode);
			ImGui::End();

			EDITOR_TRACE("GUI: scene_entity_modify end (PLAY MODE)");
			return;
		}

		displayEditorMode(isPlayMode);
		ImGui::End();

		EDITOR_TRACE("GUI: scene_entity_modify");
	}

	void GUI_EntityPanel::displayEditorMode(bool is_play_mode) {
		if (currentEntity->hasComponent<ecs::TagComponent>()) {
			Scene_Handle_TagComponent();
		}

		if (currentEntity->hasComponent<ecs::TransformComponent>()) {
			Scene_Handle_TransformComponent();
		}

		if (currentEntity->hasComponent<ecs::ScriptComponent>()) {
			Scene_Handle_ScriptComponent();
		}

		if (currentEntity->hasComponent<ecs::RenderableComponent>()) {
			Scene_Handle_RenderableComponent();
		}

		if (currentEntity->hasComponent<ecs::CameraComponent>()) {
			Scene_Handle_CameraComponent(is_play_mode);
		}

		if (currentEntity->hasComponent<ecs::ColorComponent>()) {
			Scene_Handle_ColorComponent();
		}

		if (currentEntity->hasComponent<ecs::Texture2DComponent>()) {
			Scene_Handle_Texture2DComponent();
		}

		if (currentEntity->hasComponent<ecs::TextureCubemapComponent>()) {
			Scene_Handle_TextureCubemapComponent();
		}

		if (currentEntity->hasComponent<ecs::LightComponent>()) {
			Scene_Handle_LightComponent();
		}
			
		Scene_Entity_Modify_PopUp();
	}

	void GUI_EntityPanel::displayPlayMode(bool is_play_mode) {
		ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

		if (currentEntity->hasComponent<ecs::TransformComponent>()) {
			Scene_Handle_TransformComponent();
		}

		if (currentEntity->hasComponent<ecs::CameraComponent>())
			Scene_Handle_CameraComponent(is_play_mode);

		if (currentEntity->hasComponent<ecs::ColorComponent>()) {
			Scene_Handle_ColorComponent();
		}

		if (currentEntity->hasComponent<ecs::LightComponent>()) {
			Scene_Handle_LightComponent();
		}
	}

	void GUI_EntityPanel::Scene_Entity_Modify_PopUp() {
		if (ImGui::IsWindowFocused()) {
			if (window::Input::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("SceneEntityModifyPopUp");
			}
		}

		// Actual PopUp menu
		{
			const bool hasNotColorOrTexture = !currentEntity->hasComponent<ecs::ColorComponent>()
				&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
				&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

			if (ImGui::BeginPopup("SceneEntityModifyPopUp")) {
				if (ImGui::BeginMenu("Add Component")) {
					if (!currentEntity->hasComponent<ecs::RenderableComponent>()) {
						if (ImGui::MenuItem("Add RenderableComponent")) {
							currentEntity->addComponent<ecs::RenderableComponent>(ECS_RENDERABLE);
						}
					}
						
					if (hasNotColorOrTexture) {
						if (ImGui::BeginMenu("Add Color/Texture")) {
							if (ImGui::MenuItem("Add Texture2DComponent")) {
								currentEntity->addComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D);
								ecs::SceneEvents::Instance().updatedTexture2D(currentEntity);
							}
								
							if (ImGui::MenuItem("Add TextureCubemapComponent")) {
								currentEntity->addComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP);
								ecs::SceneEvents::Instance().updatedCubemap(currentEntity);
							}
								
							if (ImGui::MenuItem("Add ColorComponent")) {
								currentEntity->addComponent<ecs::ColorComponent>(ECS_COLOR);
								ecs::SceneEvents::Instance().updatedColor(currentEntity);
							}

							ImGui::EndMenu();
						}
					}

					if (!currentEntity->hasComponent<ecs::LightComponent>()) {
						if (ImGui::MenuItem("Add LightComponent")) {
							currentEntity->addComponent<ecs::LightComponent>(ECS_LIGHT);
							ecs::SceneEvents::Instance().updatedLight(currentEntity);
						}
					}

					if (!currentEntity->hasComponent<ecs::CameraComponent>()) {
						if (ImGui::MenuItem("Add CameraComponent")) {
							currentEntity->addComponent<ecs::CameraComponent>(ECS_CAMERA);
							ecs::SceneEvents::Instance().updatedCamera(currentEntity);
						}
					}

					if (!currentEntity->hasComponent<ecs::ScriptComponent>()) {
						if (ImGui::MenuItem("Add ScriptComponent")) {
							currentEntity->addComponent<ecs::ScriptComponent>(ECS_SCRIPT);
							ecs::SceneEvents::Instance().updatedScript(currentEntity);
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		EDITOR_TRACE("GUI: scene_entity_modify_popup");
	}

	void GUI_EntityPanel::Scene_Handle_TagComponent() {
		ImGui::Text("TagComponent\n");

		auto& tag = currentEntity->getComponent<ecs::TagComponent>();
		static char entityName[50]{ "" };

		std::copy(tag.tag.begin(), tag.tag.end(), entityName);

		if (ImGui::InputText(" - tag", entityName, 50)) {
			tag.tag = std::string(entityName);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling tag component");
	}

	void GUI_EntityPanel::Scene_Handle_TransformComponent() {
		ImGui::Separator();
		ImGui::Text("TransformComponent\n");

		auto& tran = currentEntity->getComponent<ecs::TransformComponent>();

		// Sliders
		{
			bool updatedTransform = false;

			static float lastGeneral;
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
				tran.recalculate();
				ecs::SceneEvents::Instance().updateTransform(currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
	}

	void GUI_EntityPanel::Scene_Handle_ScriptComponent() {
		ImGui::Separator();

		ImGui::Text("ScriptComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Script")) {
			currentEntity->removeComponent<ecs::ScriptComponent>(ECS_SCRIPT);
			GUI_TextEditor::Instance()->reset();
			return;
		}

		const auto& script = currentEntity->getComponent<ecs::ScriptComponent>();
		ImGui::Text("Current script: %s", script.script);

		if (ImGui::Button("Create new script")) { GUI_TextEditor::Instance()->setCreatingNewScript(); }
		
		ImGui::SameLine();

		if (ImGui::Button("Load from file")) { GUI_TextEditor::Instance()->setLoadingScript(); }

		ImGui::SameLine();

		if (ImGui::Button("Assign script to entity")) { GUI_Filesystem::Instance()->openAssigningScriptWindow(); }

		GUI_Filesystem::Instance()->displayAssigningScriptWindow(currentEntity);

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling script component");
	}

	void GUI_EntityPanel::Scene_Handle_RenderableComponent() {
		static bool GUI_modify_renderable = false; // should display option to modify the whole RenderableComponent ?

		auto& renderable = currentEntity->getComponent<ecs::RenderableComponent>();

		ImGui::Separator();
		ImGui::Text("RenderableComponent\n");
		ImGui::SameLine();

		static bool color_texture_OR;
		color_texture_OR = !currentEntity->hasComponent<ecs::ColorComponent>() 
			|| !currentEntity->hasComponent<ecs::Texture2DComponent>()
			|| !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

		if (color_texture_OR)
			if (ImGui::MenuItem("Remove Renderable")) {
				currentEntity->removeComponent<ecs::RenderableComponent>(ECS_RENDERABLE);
				ecs::SceneEvents::Instance().updateRenderables(currentEntity);
				return;
			}

		ImGui::Text("Type: %s", renderable.name.c_str());

		static bool color_texture_AND;
		color_texture_AND = !currentEntity->hasComponent<ecs::ColorComponent>() 
			&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
			&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

		if (color_texture_AND)
			ImGui::Text("WARNING: Object will not be rendered until you will add Color or Texture!");

		if (renderable.vertices.empty())
			GUI_modify_renderable = true;

		if (GUI_modify_renderable) {
			if (Button_ChooseRenderable<graphics::MeshCreator::Cube>(renderable, "Cube")) {
				GUI_modify_renderable = false;
			}

			ImGui::SameLine();

			if (Button_ChooseRenderable<graphics::MeshCreator::Pyramid>(renderable, "Pyramid")) {
				GUI_modify_renderable = false;
			}

			ImGui::SameLine();

			if (Button_ChooseRenderable<graphics::MeshCreator::Wall>(renderable, "Wall")) {
				GUI_modify_renderable = false;
			}

			ImGui::SameLine();

			if (Button_ChooseRenderable<graphics::MeshCreator::Surface>(renderable, "Surface")) {
				GUI_modify_renderable = false;
			}

			if (ImGui::Button("Do not modify")) {
				GUI_modify_renderable = false;
			}
		}
		else {
			ImGui::SameLine();
			if (ImGui::MenuItem(" --- Modify Renderable"))
				GUI_modify_renderable = true;
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling renderable component");
	}

	void GUI_EntityPanel::Scene_Handle_CameraComponent(bool is_play_mode) {
		static char* GUI_input{ (char*)"empty" };
		auto& camcmp = currentEntity->getComponent<ecs::CameraComponent>();

		ImGui::Separator();
		ImGui::Text("CameraComponent\n");
		ImGui::SameLine();
		if (camcmp.id.find("main") == std::string::npos) {
			if (ImGui::Button("Remove Camera")) {
				currentEntity->removeComponent<ecs::CameraComponent>(ECS_CAMERA);
				return;
			}
		}

		ImGui::Text("WARNING: If you want to use this camera, make sure\nthat it is the only with \"main\" CameraID!");

		GUI_input = (char*)camcmp.id.c_str();
		if (ImGui::InputText("CameraID", GUI_input, 30))
			camcmp.id = std::string(GUI_input);

		ImGui::Checkbox("Perspective (TRUE) / Orthographic (FALSE)", &camcmp.Perspective);

		static bool updated_camera;
		updated_camera = false;

		if (camcmp.Perspective) {
			if (ImGui::DragFloat("AspectRatio", &camcmp.p_aspectRatio, 0.1f, 1.f, 10.f)) updated_camera = true;
			if (ImGui::Button("Set 16:9")) camcmp.p_aspectRatio = 16.f / 9.f;
			ImGui::SameLine();
			if (ImGui::Button("Set 8:5")) camcmp.p_aspectRatio = 8.f / 5.f;
			ImGui::SameLine();
			if (ImGui::Button("Set 4:3")) camcmp.p_aspectRatio = 4.f / 3.f;

			if (ImGui::DragFloat("Field Of View", &camcmp.p_fov, 0.1f, 1.f, 90.f) ) updated_camera = true;
			if (ImGui::DragFloat("Near", &camcmp.p_near, 0.01f, 0.001f, 150.f)	  ) updated_camera = true;
			if (ImGui::DragFloat("Far", &camcmp.p_far, 0.1f, 0.001f, 150.f)		  ) updated_camera = true;
		}
		else {
			if (ImGui::DragFloat("Left", &camcmp.o_left, 0.1f, -100.f, 100.f)		) updated_camera = true;
			if (ImGui::DragFloat("Right", &camcmp.o_right, 0.1f, -100.f, 100.f)		) updated_camera = true;
			if (ImGui::DragFloat("Top", &camcmp.o_top, 0.1f, -100.f, 100.f)			) updated_camera = true;
			if (ImGui::DragFloat("Bottom", &camcmp.o_bottom, 0.1f, -100.f, 100.f)	) updated_camera = true;
			if (ImGui::DragFloat("Near", &camcmp.o_near, 0.1f, 0.001f, 150.f)		) updated_camera = true;
			if (ImGui::DragFloat("Far", &camcmp.o_far, 0.1f, 0.001f, 150.f)			) updated_camera = true;
		}

		if (updated_camera)
			if(!is_play_mode) ecs::SceneEvents::Instance().updatedCamera(currentEntity);

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling camera component");
	}

	void GUI_EntityPanel::Scene_Handle_ColorComponent() {
		ImGui::Separator();
		ImGui::Text("ColorComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Color")) {
			currentEntity->removeComponent<ecs::ColorComponent>(ECS_COLOR);
			ecs::SceneEvents::Instance().updatedColor(currentEntity);
			return;
		}

		auto& color = currentEntity->getComponent<ecs::ColorComponent>();

		if (ImGui::ColorEdit3("- color", maths::vec3::value_ptr_nonconst(color.texture))) {
			ecs::SceneEvents::Instance().updatedColor(currentEntity);
		}
			
		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
	}

	void GUI_EntityPanel::Scene_Handle_Texture2DComponent() {
		static bool GUI_load_new_texture = false;
		auto& tex = currentEntity->getComponent<ecs::Texture2DComponent>();

		ImGui::Separator();
		ImGui::Text("Texture2DComponent\n");
		ImGui::SameLine();
		if (ImGui::MenuItem("Remove Texture")) {
			currentEntity->removeComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D);
			ecs::SceneEvents::Instance().updatedTexture2D(currentEntity);
			return;
		}

		if (platforms::TextureOpenGL::hasTexture(tex.texture)) {
			ImGui::Image((void*)platforms::TextureOpenGL::getTexture(tex.texture), ImVec2{ 100.f, 100.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		ImGui::Text("Current Texture: %s", tex.texture.c_str());

		if (tex.texture != "empty") {
			if (!GUI_load_new_texture) {
				if (ImGui::Button("Load new Texture menu"))
					GUI_load_new_texture = true;
			}
			else {
				if (ImGui::Button("Close Texture Menu"))
					GUI_load_new_texture = false;
			}
		}
		else {
			GUI_load_new_texture = true;
		}

		if (!GUI_load_new_texture) {
			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component (without loading new tex)");
			return;
		}

		static bool first_init = true;
		static char input[50];

		if (first_init) { 
			strcpy_s(input, tex.texture.c_str()); 
			first_init = false; 
		}

		ImGui::InputText(" ex. .jpg / .png", input, 50);

		ImGui::Text("Texture, which will be loaded: %s", input);
		ImGui::Text("WARNING: if texture do not exist, no shape will appear!");

		if (ImGui::Button("Load Texture")) {
			tex.texture = std::string(input);
			ecs::SceneEvents::Instance().updatedTexture2D(currentEntity);
			GUI_load_new_texture = false;
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component");
	}

	void GUI_EntityPanel::Scene_Handle_TextureCubemapComponent() {
		static bool GUI_load_new_cubemap = false;
		auto& cubemap = currentEntity->getComponent<ecs::TextureCubemapComponent>();

		ImGui::Separator();
		ImGui::Text("CubemapComponent\n");
		ImGui::SameLine();
		if (ImGui::MenuItem("Remove Texture")) {
			currentEntity->removeComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP);
			ecs::SceneEvents::Instance().updatedCubemap(currentEntity);
			return;
		}

		ImGui::Text("Current Cubemap: %s", cubemap.texture.c_str());

		if (cubemap.texture != "empty") {
			if (!GUI_load_new_cubemap) {
				if (ImGui::Button("Load new Texture menu"))
					GUI_load_new_cubemap = true;
			}
			else {
				if (ImGui::Button("Close Texture Menu"))
					GUI_load_new_cubemap = false;
			}
		}
		else {
			GUI_load_new_cubemap = true;
		}

		if (!GUI_load_new_cubemap) {
			EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component (without loading new tex)");
			return;
		}

		static bool first_init = true;
		static char input[50];

		if (first_init) {
			strcpy_s(input, cubemap.texture.c_str());
			first_init = false;
		}

		ImGui::InputText(" ex. .jpg / .png", input, 50);

		ImGui::Text("Cubemap, which will be loaded: %s", cubemap.texture.c_str());
		ImGui::Text("WARNING: if cubemap do not exist, no shape will appear!");

		if (ImGui::Button("Load Cubemap")) {
			cubemap.texture = std::string(input);
			ecs::SceneEvents::Instance().updatedCubemap(currentEntity);
			GUI_load_new_cubemap = false;
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component");
	}

	void GUI_EntityPanel::Scene_Handle_LightComponent() {
		ImGui::Separator();

		ImGui::Text("LightComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Light")) {
			currentEntity->removeComponent<ecs::LightComponent>(ECS_LIGHT);
			ecs::SceneEvents::Instance().updatedLight(currentEntity);
			return;
		}

		auto& light = currentEntity->getComponent<ecs::LightComponent>();
		bool updatedLight = false;

		if (ImGui::DragFloat3("Ambient Light", &light.ambient.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
		if (ImGui::DragFloat3("Diffuse Light", &light.diffuse.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
		if (ImGui::DragFloat3("Specular Light", &light.specular.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
																					  						  
		if (ImGui::DragFloat("Constant", &light.constant, 0.001f, 0.f, 2.f)			) { updatedLight = true; }
		if (ImGui::DragFloat("Linear", &light.linear, 0.001f, 0.f, 0.5f)			) { updatedLight = true; }
		if (ImGui::DragFloat("Quadratic", &light.quadratic, 0.001f, 0.f, 0.1f)		) { updatedLight = true; }
		if (ImGui::DragFloat("Shininess", &light.shininess, 0.5f, 0.f, 256.f)		) { updatedLight = true; }

		if (updatedLight) {
			ecs::SceneEvents::Instance().updatedLight(currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling light component");
	}

	// --------------------------------------------
	// TEMPLATES TO WRITE MORE REUSABLE CODE
	// --------------------------------------------

	template<typename T>
	bool GUI_EntityPanel::Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName) {
		if (ImGui::Button(buttonName)) {
			renderable.name = T::getID();
			renderable.vertices = T::getVertices();
			renderable.indices = T::getIndices();

			ecs::SceneEvents::Instance().updateRenderables(currentEntity);

			return true;
		}

		return false;
	}


}
