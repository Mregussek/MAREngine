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


#include "EntityWidgetPanel.h"
#include "CommonComponentHandler.h"
#include "../../WidgetEvents/EntityWidgetEvents.h"
#include "../../EditorLogging.h"
#include "../../../Window/Window.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Core/graphics/Mesh/MeshCreator.h"
#include "../../../Core/events/SceneEvents.h"
#include "../../../Core/ecs/SceneManager.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Components/Components.h"


namespace marengine {


	WEntityWidgetPanel* WEntityWidgetPanel::Instance{ nullptr };


	void WEntityWidgetPanel::create() {
		Instance = this;
	}

	void WEntityWidgetPanel::reset() {
		currentEntity = nullptr;
	}

	void WEntityWidgetPanel::setCurrentEntity(const Entity& entity) { 
		currentEntity = &entity; 
	}

	const Entity& WEntityWidgetPanel::getCurrentEntity() const { 
		return *currentEntity;
	}

	void WEntityWidgetPanel::updateFrame() {
		ImGui::Begin("Entity Modification");

		if (!currentEntity) {
			ImGui::Text("No entity selected!");
			ImGui::End();
			return;
		}

		if (SceneManager::Instance->isPlayMode()) {
			displayPlayMode();
			ImGui::End();

			EDITOR_TRACE("GUI: scene_entity_modify end (PLAY MODE)");
			return;
		}

		displayEditorMode();
		ImGui::End();

		EDITOR_TRACE("GUI: scene_entity_modify");
	}

	void WEntityWidgetPanel::displayEditorMode() const {
		if (ImGui::CollapsingHeader("TagComponent")) {
			auto& tag = currentEntity->getComponent<TagComponent>();
			CommonComponentHandler::handleTagComponent(tag);
		}

		if (ImGui::CollapsingHeader("TransformComponent")) {
			handleTransformComponent();
		}

		if (currentEntity->hasComponent<ScriptComponent>() && ImGui::CollapsingHeader("ScriptComponent")) {
			CommonComponentHandler::handleScriptComponent(*currentEntity);
		}

		if (currentEntity->hasComponent<RenderableComponent>() && ImGui::CollapsingHeader("RenderableComponent")) {
			handleRenderableComponent();
		}

		if (currentEntity->hasComponent<CameraComponent>() && ImGui::CollapsingHeader("CameraComponent")) {
			handleCameraComponent();
		}

		if (currentEntity->hasComponent<ColorComponent>() && ImGui::CollapsingHeader("ColorComponent")) {
			handleColorComponent();
		}

		if (currentEntity->hasComponent<Texture2DComponent>() && ImGui::CollapsingHeader("Texture2DComponent")) {
			handleTexture2DComponent();
		}

		if (currentEntity->hasComponent<TextureCubemapComponent>() && ImGui::CollapsingHeader("TextureCubemapComponent")) {
			handleTextureCubemapComponent();
		}

		if (currentEntity->hasComponent<LightComponent>() && ImGui::CollapsingHeader("LightComponent")) {
			handleLightComponent();
		}
			
		popUpMenu();
	}

	void WEntityWidgetPanel::displayPlayMode() const {
		ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

		if (currentEntity->hasComponent<TransformComponent>()) {
			handleTransformComponent();
		}

		if (currentEntity->hasComponent<CameraComponent>())
			handleCameraComponent();

		if (currentEntity->hasComponent<ColorComponent>()) {
			handleColorComponent();
		}

		if (currentEntity->hasComponent<LightComponent>()) {
			handleLightComponent();
		}
	}

	void WEntityWidgetPanel::popUpMenu() const {
		if (ImGui::IsWindowFocused()) {
			if (Window::isMousePressed(MAR_MOUSE_BUTTON_2)) {
				ImGui::OpenPopup("SceneEntityModifyPopUp");
			}
		}

		{ // Actual PopUp menu
			if (ImGui::BeginPopup("SceneEntityModifyPopUp")) {
				if (ImGui::BeginMenu("Add Component")) {
					const bool hasRenderable{ currentEntity->hasComponent<RenderableComponent>() };
					const bool hasLight{ currentEntity->hasComponent<LightComponent>() };
					const bool hasCamera{ currentEntity->hasComponent<CameraComponent>() };
					const bool hasScript{ currentEntity->hasComponent<ScriptComponent>() };
					const bool hasNeitherColorNorTexture = !currentEntity->hasComponent<ColorComponent>() 
						&& !currentEntity->hasComponent<Texture2DComponent>() 
						&& !currentEntity->hasComponent<TextureCubemapComponent>();

					if (!hasRenderable && ImGui::MenuItem("Add RenderableComponent")) {
						FEntityWidgetEvents::onAddComponent<RenderableComponent>(*currentEntity);
					}
						
					if (hasNeitherColorNorTexture) {
						if (ImGui::MenuItem("Add ColorComponent")) {
							FEntityWidgetEvents::onAddComponent<ColorComponent>(*currentEntity);
						}

						if (ImGui::MenuItem("Add Texture2DComponent")) {
							FEntityWidgetEvents::onAddComponent<Texture2DComponent>(*currentEntity);
						}
								
						if (ImGui::MenuItem("Add TextureCubemapComponent")) {
							FEntityWidgetEvents::onAddComponent<TextureCubemapComponent>(*currentEntity);
						}
					}

					if (!hasLight && ImGui::MenuItem("Add LightComponent")) {
						currentEntity->addComponent<LightComponent>();
						SceneEvents::Instance().onLightAdd();
					}

					if (!hasCamera && ImGui::MenuItem("Add CameraComponent")) {
						currentEntity->addComponent<CameraComponent>();
						SceneEvents::Instance().onCameraAdd();
					}

					if (!hasScript && ImGui::MenuItem("Add ScriptComponent")) {
						currentEntity->addComponent<ScriptComponent>();
						SceneEvents::Instance().onScriptAdd();
					}

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}
	}

	void WEntityWidgetPanel::handleTransformComponent() const {
		auto& tran = currentEntity->getComponent<TransformComponent>();

		{ // Sliders
			bool updatedTransform = false;

			if (ImGui::DragFloat3("Position", &tran.center.x, 0.05f, -200.0f, 200.0f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Rotation", &tran.angles.x, 0.5f, -360.f, 360.f, "%.2f", 1.f)) { updatedTransform = true; }
			if (ImGui::DragFloat3("Scale", &tran.scale.x, 0.01f, 0.f, 20.0f, "%.2f", 1.f)) { updatedTransform = true; }

			if (updatedTransform) {
				SceneEvents::Instance().onTransformUpdate(*currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
	}

	void WEntityWidgetPanel::handleRenderableComponent() const {
		auto& renderable = currentEntity->getComponent<RenderableComponent>();

		const bool hasNeitherColorNorTexture = !currentEntity->hasComponent<ColorComponent>()
			&& !currentEntity->hasComponent<Texture2DComponent>()
			&& !currentEntity->hasComponent<TextureCubemapComponent>();

		auto modifyRenderableButtons = [this, &renderable]() {
			Button_ChooseRenderable<MeshCreator::Cube>(renderable, "Cube");
			ImGui::SameLine();
			Button_ChooseRenderable<MeshCreator::Pyramid>(renderable, "Pyramid");
			ImGui::SameLine();
			Button_ChooseRenderable<MeshCreator::Wall>(renderable, "Wall");
			ImGui::SameLine();
			Button_ChooseRenderable<MeshCreator::Surface>(renderable, "Surface");
		};
		
		// Actual Panel for Renderable
		if (hasNeitherColorNorTexture) {
			if (ImGui::MenuItem("Remove Renderable")) {
				currentEntity->removeComponent<RenderableComponent>();
				SceneEvents::Instance().onRenderableRemove();
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

	void WEntityWidgetPanel::handleCameraComponent() const {		
		auto& camera = currentEntity->getComponent<CameraComponent>();

		if (!camera.checkIfMain()) {
			if (ImGui::Button("Remove Camera")) {
				currentEntity->removeComponent<CameraComponent>();
				SceneEvents::Instance().onCameraRemove();
				return;
			}
		}

		ImGui::Text("WARNING: To use camera in PlayMode please set Camera ID to \"main\"!");

		constexpr size_t inputSize = 50;
		static char cameraID[inputSize]{ "" };

		std::fill(std::begin(cameraID), std::end(cameraID), '\0');
		std::copy(camera.id.begin(), camera.id.end(), cameraID);

		if (ImGui::InputText(" - camera ID", cameraID, inputSize)) { camera.id = std::string(cameraID); }

		ImGui::Checkbox("Perspective (True) / Orthographic (False)", &camera.Perspective);

		bool updatedCamera = false;

		if (camera.Perspective) {
			if (ImGui::DragFloat("AspectRatio", &camera.p_aspectRatio, 0.1f, 1.f, 10.f)) { updatedCamera = true; }
			if (ImGui::DragFloat("Field Of View", &camera.p_fov, 0.1f, 1.f, 90.f)) { updatedCamera = true; }
			if (ImGui::DragFloat("Near", &camera.p_near, 0.01f, 0.001f, 150.f))	{ updatedCamera = true; }
			if (ImGui::DragFloat("Far", &camera.p_far, 0.1f, 0.001f, 10000.f)) { updatedCamera = true; }
			if (ImGui::Button("Set 16:9")) { 
				camera.p_aspectRatio = 16.f / 9.f; 
				updatedCamera = true; 
			}
			ImGui::SameLine();			  							  
			if (ImGui::Button("Set 8:5")) { 
				camera.p_aspectRatio = 8.f / 5.f; 
				updatedCamera = true;
			}
			ImGui::SameLine();			  					  
			if (ImGui::Button("Set 4:3")) { 
				camera.p_aspectRatio = 4.f / 3.f;
				updatedCamera = true;
			}
		}
		else {
			if (ImGui::DragFloat("Left", &camera.o_left, 0.1f, -100.f, 100.f)	)  { updatedCamera = true; }
			if (ImGui::DragFloat("Right", &camera.o_right, 0.1f, -100.f, 100.f)	)  { updatedCamera = true; }
			if (ImGui::DragFloat("Top", &camera.o_top, 0.1f, -100.f, 100.f)		)  { updatedCamera = true; }
			if (ImGui::DragFloat("Bottom", &camera.o_bottom, 0.1f, -100.f, 100.f)) { updatedCamera = true; }
			if (ImGui::DragFloat("Near", &camera.o_near, 0.1f, 0.001f, 150.f)	)  { updatedCamera = true; }
			if (ImGui::DragFloat("Far", &camera.o_far, 0.1f, 0.001f, 150.f)		)  { updatedCamera = true; }
		}

		if (updatedCamera) {
			if (camera.checkIfMain()) {
				SceneEvents::Instance().onMainCameraUpdate(*currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling camera component");
	}

	void WEntityWidgetPanel::handleColorComponent() const {
		if (ImGui::MenuItem("Remove Color")) {
			currentEntity->removeComponent<ColorComponent>();
			SceneEvents::Instance().onColorRemove();
			return;
		}

		auto& color = currentEntity->getComponent<ColorComponent>();

		if (ImGui::ColorEdit4("- color", &color.texture.x)) {
			SceneEvents::Instance().onColorUpdate(*currentEntity);
		}
			
		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
	}

	void WEntityWidgetPanel::handleTexture2DComponent() const {
		auto& texture2D = currentEntity->getComponent<Texture2DComponent>();

		auto modifyCurrentTexture = [&texture = texture2D.texture, this]() {
			constexpr size_t inputSize{ 50 };
			static char input[inputSize];
			std::fill(std::begin(input), std::end(input), '\0');
			std::copy(texture.begin(), texture.end(), input);

			if (ImGui::InputText(" ex. .jpg / .png", input, inputSize)) {
				texture = std::string(input);
			}

			if (ImGui::Button("Load Texture")) {
				SceneEvents::Instance().onTexture2DUpdate(currentEntity);
			}
		};

		// Actual Texture2D Panel
		if (ImGui::MenuItem("Remove Texture")) {
			currentEntity->removeComponent<Texture2DComponent>();
			SceneEvents::Instance().onTexture2DRemove();
			return;
		}

		ImGui::Text("Current Texture: %s", texture2D.texture.c_str());

		if (TextureOpenGL::hasTexture(texture2D.texture)) {
			ImGui::Image((ImTextureID)TextureOpenGL::getTexture(texture2D.texture), { 100.f, 100.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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

	void WEntityWidgetPanel::handleTextureCubemapComponent() const {
		auto& cubemap = currentEntity->getComponent<TextureCubemapComponent>();

		auto modifyCurrentCubemap = [&texture = cubemap.texture, this]() {
			constexpr size_t inputSize{ 50 };
			static char input[inputSize];
			std::fill(std::begin(input), std::end(input), '\0');
			std::copy(texture.begin(), texture.end(), input);

			if (ImGui::InputText(" - path", input, inputSize)) {
				texture = std::string(input);
			}

			if (ImGui::Button("Load Cubemap")) {
				SceneEvents::Instance().onTextureCubemapUpdate(currentEntity);
			}
		};

		// Actual TextureCubemap Panel
		if (ImGui::MenuItem("Remove Cubemap")) {
			currentEntity->removeComponent<TextureCubemapComponent>();
			SceneEvents::Instance().onTextureCubemapRemove();
			return;
		}

		ImGui::Text("Current Cubemap: %s", cubemap.texture.c_str());

		if (TextureOpenGL::hasCubemap(cubemap.texture)) {
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

	void WEntityWidgetPanel::handleLightComponent() const {
		if (ImGui::MenuItem("Remove Light")) {
			currentEntity->removeComponent<LightComponent>();
			SceneEvents::Instance().onLightRemove();
			return;
		}

		auto& light = currentEntity->getComponent<LightComponent>();
		bool updatedLight = false;

		if (ImGui::DragFloat3("Ambient Light", &light.ambient.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
		if (ImGui::DragFloat3("Diffuse Light", &light.diffuse.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
		if (ImGui::DragFloat3("Specular Light", &light.specular.x, 0.01f, 0.f, 1.f)	) { updatedLight = true; }
																					  						  
		if (ImGui::DragFloat("Constant", &light.constant, 0.001f, 0.f, 2.f)			) { updatedLight = true; }
		if (ImGui::DragFloat("Linear", &light.linear, 0.001f, 0.f, 0.5f)			) { updatedLight = true; }
		if (ImGui::DragFloat("Quadratic", &light.quadratic, 0.001f, 0.f, 0.1f)		) { updatedLight = true; }
		if (ImGui::DragFloat("Shininess", &light.shininess, 0.5f, 0.f, 256.f)		) { updatedLight = true; }

		if (updatedLight) {
			SceneEvents::Instance().onLightUpdate(*currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling light component");
	}

	// --------------------------------------------
	// TEMPLATES TO WRITE MORE REUSABLE CODE
	// --------------------------------------------

	template<typename T>
	bool WEntityWidgetPanel::Button_ChooseRenderable(RenderableComponent& renderable, const char* buttonName) const {
		if (ImGui::Button(buttonName)) {
			renderable.name = T::getID();
			renderable.vertices = T::getVertices();
			renderable.indices = T::getIndices();

			SceneEvents::Instance().onRenderableUpdate(currentEntity);

			return true;
		}

		return false;
	}


}
