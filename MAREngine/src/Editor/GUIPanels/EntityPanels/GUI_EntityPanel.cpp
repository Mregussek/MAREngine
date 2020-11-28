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
#include "../../../Window/Window.h"


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

	void GUI_EntityPanel::setCurrentEntity(const ecs::Entity& entity) { 
		currentEntity = &entity; 
	}

	const ecs::Entity& GUI_EntityPanel::getCurrentEntity() const { 
		return *currentEntity;
	}

	void GUI_EntityPanel::update(bool isPlayMode) const {
		ImGui::Begin("Entity Modification");

		if (!currentEntity) {
			ImGui::Text("No entity selected!");
			ImGui::End();
			return;
		}

		if (isPlayMode) {
			displayPlayMode();
			ImGui::End();

			EDITOR_TRACE("GUI: scene_entity_modify end (PLAY MODE)");
			return;
		}

		displayEditorMode();
		ImGui::End();

		EDITOR_TRACE("GUI: scene_entity_modify");
	}

	void GUI_EntityPanel::displayEditorMode() const {
		if (currentEntity->hasComponent<ecs::TagComponent>()) {
			handleTagComponent();
		}

		if (currentEntity->hasComponent<ecs::TransformComponent>()) {
			handleTransformComponent();
		}

		if (currentEntity->hasComponent<ecs::ScriptComponent>()) {
			handleScriptComponent();
		}

		if (currentEntity->hasComponent<ecs::RenderableComponent>()) {
			handleRenderableComponent();
		}

		if (currentEntity->hasComponent<ecs::CameraComponent>()) {
			handleCameraComponent();
		}

		if (currentEntity->hasComponent<ecs::ColorComponent>()) {
			handleColorComponent();
		}

		if (currentEntity->hasComponent<ecs::Texture2DComponent>()) {
			handleTexture2DComponent();
		}

		if (currentEntity->hasComponent<ecs::TextureCubemapComponent>()) {
			handleTextureCubemapComponent();
		}

		if (currentEntity->hasComponent<ecs::LightComponent>()) {
			handleLightComponent();
		}
			
		popUpMenu();
	}

	void GUI_EntityPanel::displayPlayMode() const {
		ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

		if (currentEntity->hasComponent<ecs::TransformComponent>()) {
			handleTransformComponent();
		}

		if (currentEntity->hasComponent<ecs::CameraComponent>())
			handleCameraComponent();

		if (currentEntity->hasComponent<ecs::ColorComponent>()) {
			handleColorComponent();
		}

		if (currentEntity->hasComponent<ecs::LightComponent>()) {
			handleLightComponent();
		}
	}

	void GUI_EntityPanel::popUpMenu() const {
		if (ImGui::IsWindowFocused()) {
			if (window::Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("SceneEntityModifyPopUp");
			}
		}

		{ // Actual PopUp menu
			const bool hasNeitherColorNorTexture = !currentEntity->hasComponent<ecs::ColorComponent>()
				&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
				&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

			if (ImGui::BeginPopup("SceneEntityModifyPopUp")) {
				if (ImGui::BeginMenu("Add Component")) {
					if (!currentEntity->hasComponent<ecs::RenderableComponent>()) {
						if (ImGui::MenuItem("Add RenderableComponent")) {
							currentEntity->addComponent<ecs::RenderableComponent>();
						}
					}
						
					if (hasNeitherColorNorTexture) {
						if (ImGui::BeginMenu("Add Color/Texture")) {
							if (ImGui::MenuItem("Add ColorComponent")) {
								currentEntity->addComponent<ecs::ColorComponent>();
								ecs::SceneEvents::Instance().onColorAdd();
							}

							if (ImGui::MenuItem("Add Texture2DComponent")) {
								currentEntity->addComponent<ecs::Texture2DComponent>();
								ecs::SceneEvents::Instance().onTexture2DAdd();
							}
								
							if (ImGui::MenuItem("Add TextureCubemapComponent")) {
								currentEntity->addComponent<ecs::TextureCubemapComponent>();
								ecs::SceneEvents::Instance().onTextureCubemapAdd();
							}
								
							ImGui::EndMenu();
						}
					}

					if (!currentEntity->hasComponent<ecs::LightComponent>()) {
						if (ImGui::MenuItem("Add LightComponent")) {
							currentEntity->addComponent<ecs::LightComponent>();
							ecs::SceneEvents::Instance().onLightAdd();
						}
					}

					if (!currentEntity->hasComponent<ecs::CameraComponent>()) {
						if (ImGui::MenuItem("Add CameraComponent")) {
							currentEntity->addComponent<ecs::CameraComponent>();
							ecs::SceneEvents::Instance().onCameraAdd();
						}
					}

					if (!currentEntity->hasComponent<ecs::ScriptComponent>()) {
						if (ImGui::MenuItem("Add ScriptComponent")) {
							currentEntity->addComponent<ecs::ScriptComponent>();
							ecs::SceneEvents::Instance().onScriptAdd();
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		EDITOR_TRACE("GUI: scene_entity_modify_popup");
	}

	void GUI_EntityPanel::handleTagComponent() const {
		ImGui::Text("TagComponent\n");

		auto& tag = currentEntity->getComponent<ecs::TagComponent>();

		constexpr size_t inputSize = 50;
		static char entityName[inputSize]{ "" };

		std::fill(std::begin(entityName), std::end(entityName), '\0');
		std::copy(tag.tag.begin(), tag.tag.end(), entityName);

		if (ImGui::InputText(" - tag", entityName, inputSize)) {
			tag.tag = std::string(entityName);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling tag component");
	}

	void GUI_EntityPanel::handleTransformComponent() const {
		ImGui::Separator();
		ImGui::Text("TransformComponent\n");

		auto& tran = currentEntity->getComponent<ecs::TransformComponent>();

		{ // Sliders
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
				ecs::SceneEvents::Instance().onTransformUpdate(currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
	}

	void GUI_EntityPanel::handleScriptComponent() const {
		ImGui::Separator();

		ImGui::Text("ScriptComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Script")) {
			currentEntity->removeComponent<ecs::ScriptComponent>();
			GUI_TextEditor::Instance()->reset();
			return;
		}

		const auto& script = currentEntity->getComponent<ecs::ScriptComponent>();
		ImGui::Text("Current script: %s", script.script.c_str());

		if (ImGui::Button("Create new script")) { GUI_TextEditor::Instance()->setCreatingNewScript(); }
		
		ImGui::SameLine();

		if (ImGui::Button("Load from file")) { GUI_TextEditor::Instance()->setLoadingScript(); }

		ImGui::SameLine();

		if (ImGui::Button("Assign script to entity")) { GUI_Filesystem::Instance()->openAssigningScriptWindow(); }

		GUI_Filesystem::Instance()->displayAssigningScriptWindow(currentEntity);

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling script component");
	}

	void GUI_EntityPanel::handleRenderableComponent() const {
		auto& renderable = currentEntity->getComponent<ecs::RenderableComponent>();

		const bool hasNeitherColorNorTexture = !currentEntity->hasComponent<ecs::ColorComponent>()
			&& !currentEntity->hasComponent<ecs::Texture2DComponent>()
			&& !currentEntity->hasComponent<ecs::TextureCubemapComponent>();

		auto modifyRenderableButtons = [this, &renderable]() {
			Button_ChooseRenderable<graphics::MeshCreator::Cube>(renderable, "Cube");
			ImGui::SameLine();
			Button_ChooseRenderable<graphics::MeshCreator::Pyramid>(renderable, "Pyramid");
			ImGui::SameLine();
			Button_ChooseRenderable<graphics::MeshCreator::Wall>(renderable, "Wall");
			ImGui::SameLine();
			Button_ChooseRenderable<graphics::MeshCreator::Surface>(renderable, "Surface");
		};
		
		// Actual Panel for Renderable

		ImGui::Separator();
		ImGui::Text("RenderableComponent\n");

		ImGui::SameLine();

		if (hasNeitherColorNorTexture) {
			if (ImGui::MenuItem("Remove Renderable")) {
				currentEntity->removeComponent<ecs::RenderableComponent>();
				ecs::SceneEvents::Instance().onRenderableRemove();
				return;
			}
		}

		ImGui::Text("Type: %s", renderable.name.c_str());

		if (hasNeitherColorNorTexture) {
			ImGui::Text("WARNING: Object will be rendered black until you will add Color or Texture!");
		}
			
		if (renderable.vertices.empty()) {
			modifyRenderableButtons();
		}
		else {
			static bool wantToModifyRenderable = false;

			if (!wantToModifyRenderable) {
				if (ImGui::Button("Modify Renderable")) { wantToModifyRenderable = true; }
			}
			else {
				modifyRenderableButtons();
				ImGui::SameLine();
				if (ImGui::Button("Do not modify")) { wantToModifyRenderable = false; }
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling renderable component");
	}

	void GUI_EntityPanel::handleCameraComponent() const {
		ImGui::Separator();
		ImGui::Text("CameraComponent\n");
		ImGui::SameLine();
		
		auto& camera = currentEntity->getComponent<ecs::CameraComponent>();

		if (camera.id.find("main") == std::string::npos) {
			if (ImGui::Button("Remove Camera")) {
				currentEntity->removeComponent<ecs::CameraComponent>();
				ecs::SceneEvents::Instance().onCameraRemove();
				return;
			}
		}

		ImGui::Text("WARNING: To use camera in PlayMode please set Camera ID to \"main\"!");

		constexpr size_t inputSize = 50;
		static char cameraID[inputSize]{ "" };

		std::fill(std::begin(cameraID), std::end(cameraID), '\0');
		std::copy(camera.id.begin(), camera.id.end(), cameraID);

		if (ImGui::InputText(" - camera ID", cameraID, inputSize)) {
			camera.id = std::string(cameraID);
		}

		ImGui::Checkbox("Perspective (True) / Orthographic (False)", &camera.Perspective);

		bool updaterCamera = false;

		if (camera.Perspective) {
			if (ImGui::DragFloat("AspectRatio", &camera.p_aspectRatio, 0.1f, 1.f, 10.f)) { updaterCamera = true; }
			if (ImGui::DragFloat("Field Of View", &camera.p_fov, 0.1f, 1.f, 90.f) )	{ updaterCamera = true; }
			if (ImGui::DragFloat("Near", &camera.p_near, 0.01f, 0.001f, 150.f)	  )	{ updaterCamera = true; }
			if (ImGui::DragFloat("Far", &camera.p_far, 0.1f, 0.001f, 150.f)		  )	{ updaterCamera = true; }

			if (ImGui::Button("Set 16:9")) { camera.p_aspectRatio = 16.f / 9.f; }
			ImGui::SameLine();			  							  
			if (ImGui::Button("Set 8:5")) { camera.p_aspectRatio = 8.f / 5.f; }
			ImGui::SameLine();			  					  
			if (ImGui::Button("Set 4:3")) { camera.p_aspectRatio = 4.f / 3.f; }
		}
		else {
			if (ImGui::DragFloat("Left", &camera.o_left, 0.1f, -100.f, 100.f)	)  { updaterCamera = true; }
			if (ImGui::DragFloat("Right", &camera.o_right, 0.1f, -100.f, 100.f)	)  { updaterCamera = true; }
			if (ImGui::DragFloat("Top", &camera.o_top, 0.1f, -100.f, 100.f)		)  { updaterCamera = true; }
			if (ImGui::DragFloat("Bottom", &camera.o_bottom, 0.1f, -100.f, 100.f)) { updaterCamera = true; }
			if (ImGui::DragFloat("Near", &camera.o_near, 0.1f, 0.001f, 150.f)	)  { updaterCamera = true; }
			if (ImGui::DragFloat("Far", &camera.o_far, 0.1f, 0.001f, 150.f)		)  { updaterCamera = true; }
		}

		if (updaterCamera) {
			ecs::SceneEvents::Instance().onCameraUpdate(currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling camera component");
	}

	void GUI_EntityPanel::handleColorComponent() const {
		ImGui::Separator();
		ImGui::Text("ColorComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Color")) {
			currentEntity->removeComponent<ecs::ColorComponent>();
			ecs::SceneEvents::Instance().onColorRemove();
			return;
		}

		auto& color = currentEntity->getComponent<ecs::ColorComponent>();

		if (ImGui::ColorEdit4("- color", &color.texture.x)) {
			ecs::SceneEvents::Instance().onColorUpdate(currentEntity);
		}
			
		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
	}

	void GUI_EntityPanel::handleTexture2DComponent() const {
		typedef platforms::TextureOpenGL TexGL;
		auto& texture2D = currentEntity->getComponent<ecs::Texture2DComponent>();

		auto modifyCurrentTexture = [&texture = texture2D.texture, this]() {
			constexpr size_t inputSize{ 50 };
			static char input[inputSize];
			std::fill(std::begin(input), std::end(input), '\0');
			std::copy(texture.begin(), texture.end(), input);

			if (ImGui::InputText(" ex. .jpg / .png", input, inputSize)) {
				texture = std::string(input);
			}

			if (ImGui::Button("Load Texture")) {
				ecs::SceneEvents::Instance().onTexture2DUpdate(currentEntity);
			}
		};

		// Actual Texture2D Panel

		ImGui::Separator();
		ImGui::Text("Texture2DComponent\n");
		ImGui::SameLine();
		if (ImGui::MenuItem("Remove Texture")) {
			currentEntity->removeComponent<ecs::Texture2DComponent>();
			ecs::SceneEvents::Instance().onTexture2DRemove();
			return;
		}

		ImGui::Text("Current Texture: %s", texture2D.texture.c_str());

		if (TexGL::hasTexture(texture2D.texture)) {
			ImGui::Image((ImTextureID)TexGL::getTexture(texture2D.texture), { 100.f, 100.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			static bool wantToModifyTexture = false;
			if (!wantToModifyTexture) {
				if (ImGui::Button("Modify current Texture")) { wantToModifyTexture = true; }
			}
			else {
				modifyCurrentTexture();
				ImGui::SameLine();
				if (ImGui::Button("Do not modify")) { wantToModifyTexture = false; }
			}
		}
		else {
			modifyCurrentTexture();
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component");
	}

	void GUI_EntityPanel::handleTextureCubemapComponent() const {
		typedef platforms::TextureOpenGL TexGL;
		auto& cubemap = currentEntity->getComponent<ecs::TextureCubemapComponent>();

		auto modifyCurrentCubemap = [&texture = cubemap.texture, this]() {
			constexpr size_t inputSize{ 50 };
			static char input[inputSize];
			std::fill(std::begin(input), std::end(input), '\0');
			std::copy(texture.begin(), texture.end(), input);

			if (ImGui::InputText(" - path", input, inputSize)) {
				texture = std::string(input);
			}

			if (ImGui::Button("Load Cubemap")) {
				ecs::SceneEvents::Instance().onTextureCubemapUpdate(currentEntity);
			}
		};

		// Actual TextureCubemap Panel

		ImGui::Separator();
		ImGui::Text("TextureCubemapComponent\n");
		ImGui::SameLine();
		if (ImGui::MenuItem("Remove Cubemap")) {
			currentEntity->removeComponent<ecs::TextureCubemapComponent>();
			ecs::SceneEvents::Instance().onTextureCubemapRemove();
			return;
		}

		ImGui::Text("Current Cubemap: %s", cubemap.texture.c_str());

		if (TexGL::hasCubemap(cubemap.texture)) {
			static bool wantToModifyCubemap = false;
			if (!wantToModifyCubemap) {
				if (ImGui::Button("Modify current Texture")) { wantToModifyCubemap = true; }
			}
			else {
				modifyCurrentCubemap();
				ImGui::SameLine();
				if (ImGui::Button("Do not modify")) { wantToModifyCubemap = false; }
			}
		}
		else {
			modifyCurrentCubemap();
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component");
	}

	void GUI_EntityPanel::handleLightComponent() const {
		ImGui::Separator();

		ImGui::Text("LightComponent\n");

		ImGui::SameLine();

		if (ImGui::MenuItem("Remove Light")) {
			currentEntity->removeComponent<ecs::LightComponent>();
			ecs::SceneEvents::Instance().onLightRemove();
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
			ecs::SceneEvents::Instance().onLightUpdate(currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling light component");
	}

	// --------------------------------------------
	// TEMPLATES TO WRITE MORE REUSABLE CODE
	// --------------------------------------------

	template<typename T>
	bool GUI_EntityPanel::Button_ChooseRenderable(ecs::RenderableComponent& renderable, const char* buttonName) const {
		if (ImGui::Button(buttonName)) {
			renderable.name = T::getID();
			renderable.vertices = T::getVertices();
			renderable.indices = T::getIndices();

			ecs::SceneEvents::Instance().onRenderableUpdate(currentEntity);

			return true;
		}

		return false;
	}


}
