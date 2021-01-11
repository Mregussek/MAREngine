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
#include "../../WidgetEvents/EventsEntityWidget.h"
#include "../../../Core/ecs/Entity/EventsComponentEntity.h"
#include "../../EditorLogging.h"
#include "../../../Window/Window.h"
#include "../../../Platform/OpenGL/TextureOpenGL.h"
#include "../../../Core/graphics/Mesh/MeshCreator.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
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

		if (FSceneManagerEditor::Instance->isPlayMode()) {
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

		if (currentEntity->hasComponent<PythonScriptComponent>() && ImGui::CollapsingHeader("PythonScriptComponent")) {
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

		if (currentEntity->hasComponent<PointLightComponent>() && ImGui::CollapsingHeader("PointLightComponent")) {
			handlePointLightComponent();
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

		if (currentEntity->hasComponent<PointLightComponent>()) {
			handlePointLightComponent();
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
				if (ImGui::BeginMenu("Childs")) {
					displayChildsPopMenu();
					ImGui::EndMenu();
				}
				
				if (ImGui::BeginMenu("Components")) {
					displayComponentPopMenu();
					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}
	}

	void WEntityWidgetPanel::displayChildsPopMenu() const {
		if (ImGui::MenuItem("Create and assign child")) {
			FEventsEntityWidget::Instance->onCreateChild(*currentEntity);
		}

		if (ImGui::MenuItem("Assign new child")) {

		}
	}

	void WEntityWidgetPanel::displayComponentPopMenu() const {
		const bool hasRenderable{ currentEntity->hasComponent<RenderableComponent>() };
		const bool hasLight{ currentEntity->hasComponent<PointLightComponent>() };
		const bool hasCamera{ currentEntity->hasComponent<CameraComponent>() };
		const bool hasScript{ currentEntity->hasComponent<PythonScriptComponent>() };
		const bool hasNeitherColorNorTexture = !currentEntity->hasComponent<ColorComponent>()
			&& !currentEntity->hasComponent<Texture2DComponent>()
			&& !currentEntity->hasComponent<TextureCubemapComponent>();

		if (!hasRenderable && ImGui::MenuItem("Add RenderableComponent")) {
			FEventsComponentEntity::onAdd<RenderableComponent>(*currentEntity);
		}

		if (hasNeitherColorNorTexture) {
			if (ImGui::MenuItem("Add ColorComponent")) {
				FEventsComponentEntity::onAdd<ColorComponent>(*currentEntity);
			}

			if (ImGui::MenuItem("Add Texture2DComponent")) {
				FEventsComponentEntity::onAdd<Texture2DComponent>(*currentEntity);
			}

			if (ImGui::MenuItem("Add TextureCubemapComponent")) {
				FEventsComponentEntity::onAdd<TextureCubemapComponent>(*currentEntity);
			}
		}

		if (!hasLight && ImGui::MenuItem("Add PointLightComponent")) {
			FEventsComponentEntity::onAdd<PointLightComponent>(*currentEntity);
		}

		if (!hasCamera && ImGui::MenuItem("Add CameraComponent")) {
			FEventsComponentEntity::onAdd<CameraComponent>(*currentEntity);
		}

		if (!hasScript && ImGui::MenuItem("Add PythonScriptComponent")) {
			FEventsComponentEntity::onAdd<PythonScriptComponent>(*currentEntity);
		}
	}

	void WEntityWidgetPanel::handleTransformComponent() const {
		auto& tran = currentEntity->getComponent<TransformComponent>();

		{ // Sliders
			bool updatedTransform = false;

			if (CommonComponentHandler::drawVec3Control("Position", tran.position, 0.f, 100.f)) { updatedTransform = true; }
			if (CommonComponentHandler::drawVec3Control("Rotation", tran.rotation, 0.f, 100.f)) { updatedTransform = true; }
			if (CommonComponentHandler::drawVec3Control("Scale", tran.scale, 0.f, 100.f)) { updatedTransform = true; }

			ImGui::NewLine();

			if (updatedTransform) {
				FEventsComponentEntity::onUpdate<TransformComponent>(*currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling transform component");
	}

	void WEntityWidgetPanel::handleRenderableComponent() const {
		if (ImGui::MenuItem("Remove Renderable")) {
			FEventsComponentEntity::onRemove<RenderableComponent>(*currentEntity);
			return;
		}
		auto& renderable = currentEntity->getComponent<RenderableComponent>();

		ImGui::Text("Type: %s", renderable.name.c_str());

		bool userHasChosenRenderable{ false };
		if (Button_ChooseRenderable<MeshCreator::Cube>(renderable, "Cube")) { userHasChosenRenderable = true; }
		ImGui::SameLine();
		if (Button_ChooseRenderable<MeshCreator::Pyramid>(renderable, "Pyramid")) { userHasChosenRenderable = true; }
		ImGui::SameLine();
		if (Button_ChooseRenderable<MeshCreator::Wall>(renderable, "Wall")) { userHasChosenRenderable = true; }
		ImGui::SameLine();
		if (Button_ChooseRenderable<MeshCreator::Surface>(renderable, "Surface")) { userHasChosenRenderable = true; }

		if (userHasChosenRenderable) {
			FEventsComponentEntity::onUpdate<RenderableComponent>(*currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling renderable component");
	}

	void WEntityWidgetPanel::handleCameraComponent() const {		
		if (ImGui::Button("Remove Camera")) {
			FEventsComponentEntity::onRemove<CameraComponent>(*currentEntity);
			return;
		}
		auto& camera = currentEntity->getComponent<CameraComponent>();

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
			FEventsComponentEntity::onUpdate<CameraComponent>(*currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling camera component");
	}

	void WEntityWidgetPanel::handleColorComponent() const {
		if (ImGui::MenuItem("Remove Color")) {
			FEventsComponentEntity::onRemove<ColorComponent>(*currentEntity);
			return;
		}
		auto& color = currentEntity->getComponent<ColorComponent>();

		if (ImGui::ColorEdit4("- color", &color.color.x)) {
			FEventsComponentEntity::onUpdate<ColorComponent>(*currentEntity);
		}
			
		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling color component");
	}

	void WEntityWidgetPanel::handleTexture2DComponent() const {
		if (ImGui::MenuItem("Remove Texture")) {
			FEventsComponentEntity::onRemove<Texture2DComponent>(*currentEntity);
			return;
		}
		auto& texture2D = currentEntity->getComponent<Texture2DComponent>();

		ImGui::Text("Current Texture: %s", texture2D.texturePath.c_str());

		if (TextureOpenGL::hasTexture(texture2D.texturePath)) {
			ImGui::Image((ImTextureID)TextureOpenGL::getTexture(texture2D.texturePath), { 100.f, 100.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}

		constexpr size_t inputSize{ 128 };
		static char input[inputSize];
		std::fill(std::begin(input), std::end(input), '\0');
		std::copy(texture2D.texturePath.begin(), texture2D.texturePath.end(), input);

		if (ImGui::InputText(" ex. .jpg / .png", input, inputSize)) {
			texture2D.texturePath = std::string(input);
			
			// here need to try loading the texture, not only checking if it is already loaded
			if (TextureOpenGL::hasTexture(texture2D.texturePath)) {
				FEventsComponentEntity::onUpdate<Texture2DComponent>(*currentEntity);
			}
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling texture2D component");
	}

	void WEntityWidgetPanel::handleTextureCubemapComponent() const {
		if (ImGui::MenuItem("Remove Cubemap")) {
			FEventsComponentEntity::onRemove<TextureCubemapComponent>(*currentEntity);
			return;
		}
		auto& cubemap = currentEntity->getComponent<TextureCubemapComponent>();

		ImGui::Text("Current Cubemap: %s", cubemap.texturePath.c_str());

		constexpr size_t inputSize{ 50 };
		static char input[inputSize];
		std::fill(std::begin(input), std::end(input), '\0');
		std::copy(cubemap.texturePath.begin(), cubemap.texturePath.end(), input);

		if (ImGui::InputText(" - path", input, inputSize)) {
			cubemap.texturePath = std::string(input);
		}

		// here need to try loading the texture, not only checking if it is already loaded
		if (ImGui::Button("Load Cubemap")) {
			FEventsComponentEntity::onUpdate<TextureCubemapComponent>(*currentEntity);
		}

		EDITOR_TRACE("GUI: SELECTED-ENTITY: handling TextureCubemap component");
	}

	void WEntityWidgetPanel::handlePointLightComponent() const {
		if (ImGui::MenuItem("Remove Light")) {
			FEventsComponentEntity::onRemove<PointLightComponent>(*currentEntity);
			return;
		}
		auto& pointLight{ currentEntity->getComponent<PointLightComponent>().pointLight };
		bool updatedLight = false;

		if (CommonComponentHandler::drawVec4Control("Ambient", pointLight.ambient, 0.f, 100.f)) { updatedLight = true; }
		if (CommonComponentHandler::drawVec4Control("Diffuse", pointLight.diffuse, 0.f, 100.f)) { updatedLight = true; }
		if (CommonComponentHandler::drawVec4Control("Specular", pointLight.specular, 0.f, 100.f)) { updatedLight = true; }
							  		
		ImGui::NewLine();

		if (ImGui::DragFloat("Constant", &pointLight.constant, 0.001f, 0.f, 2.f)) { updatedLight = true; }
		if (ImGui::DragFloat("Linear", &pointLight.linear, 0.001f, 0.f, 0.5f)) { updatedLight = true; }
		if (ImGui::DragFloat("Quadratic", &pointLight.quadratic, 0.001f, 0.f, 0.1f)) { updatedLight = true; }
		if (ImGui::DragFloat("Shininess", &pointLight.shininess, 0.5f, 0.f, 256.f)) { updatedLight = true; }

		if (updatedLight) {
			FEventsComponentEntity::onUpdate<PointLightComponent>(*currentEntity);
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
			return true;
		}

		return false;
	}


}
