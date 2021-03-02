/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "SceneDebugWidget.h"
#include "../../../Core/ecs/Entity/Entity.h"
#include "../../../Core/ecs/Components/Components.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../EntityPanels/EntityWidgetPanel.h"


namespace marengine {

	WSceneDebugWidget* WSceneDebugWidget::Instance{ nullptr };


	void WSceneDebugWidget::create() {
		Instance = this;
	}

	void WSceneDebugWidget::updateFrame() {
		Scene* scene{ FSceneManagerEditor::Instance->getScene() };
		ImGui::Begin("SceneDebugWidget");

		displayInfoAbout(scene);

		ImGui::Separator();

		const FEntityArray& entities{ scene->getEntities() };
		for (const Entity& entity : entities) {
			displayInfoAbout(entity);
			ImGui::Separator();
		}

		ImGui::End();
	}

	void WSceneDebugWidget::displayInfoAbout(Scene* scene) const {
		ImGui::Text("SceneName: %s", scene->getName().c_str());

		const maths::vec3& background{ scene->getBackground() };
		ImGui::Text("SceneBackground: (%f , %f , %f)", background.x, background.y, background.z);
	}

	void WSceneDebugWidget::displayInfoAbout(const Entity& entity) const {
		if (ImGui::TreeNode(entity.getComponent<TagComponent>().tag.c_str())) {
			const auto displayVec3 = [](const char* label, const maths::vec3& v) {
				ImGui::Text(label, v.x, v.y, v.z);
			};
			const auto displayVec4 = [](const char* label, const maths::vec4& v) {
				ImGui::Text(label, v.x, v.y, v.z, v.w);
			};

			ImGui::Text("hasChilds: %i", entity.hasChilds());
			ImGui::Text("ChildsCount: %i", entity.getChilds().size());

			const auto& meshBatchInfoComponent{ entity.getComponent<MeshBatchInfoComponent>() };
			ImGui::Text("meshBatchInfoComponent.batchIndex: %i", meshBatchInfoComponent.batchIndex);
			ImGui::Text("meshBatchInfoComponent.indexAtBatch: %i", meshBatchInfoComponent.indexAtBatch);
			ImGui::Text("meshBatchInfoComponent.batchType: %i", meshBatchInfoComponent.batchType);
			ImGui::Text("meshBatchInfoComponent.beginVertices: %i", meshBatchInfoComponent.beginVertices);
			ImGui::Text("meshBatchInfoComponent.endVertices: %i", meshBatchInfoComponent.endVertices);
			ImGui::Text("meshBatchInfoComponent.beginIndices: %i", meshBatchInfoComponent.beginIndices);
			ImGui::Text("meshBatchInfoComponent.endIndices: %i", meshBatchInfoComponent.endIndices);

			const auto& lightBatchInfoComponent{ entity.getComponent<LightBatchInfoComponent>() };
			ImGui::Text("lightBatchInfoComponent.indexAtBatch: %i", lightBatchInfoComponent.indexAtBatch);
			ImGui::Text("lightBatchInfoComponent.batchType: %i", lightBatchInfoComponent.batchType);

			const auto& transformComponent{ entity.getComponent<TransformComponent>() };
			displayVec3("Transform.center: <%f , %f , %f>", transformComponent.position);
			displayVec3("Transform.rotation: <%f , %f , %f>", transformComponent.rotation);
			displayVec3("Transform.scale: <%f , %f , %f>", transformComponent.scale);

			if (entity.hasComponent<PythonScriptComponent>()) {
				const auto& pythonScriptComponent{ entity.getComponent<PythonScriptComponent>() };
				ImGui::Text("PythonScript.scriptsPath: %s", pythonScriptComponent.scriptsPath.c_str());
			}

			if (entity.hasComponent<RenderableComponent>()) {
				const auto& renderableComponent{ entity.getComponent<RenderableComponent>() };
				ImGui::Text("Renderable.name: %s", renderableComponent.name.c_str());
				ImGui::Text("Renderable.vertices.size: %i", renderableComponent.vertices.size());
				ImGui::Text("Renderable.indices.size: %i", renderableComponent.indices.size());
			}

			if (entity.hasComponent<CameraComponent>()) {
				const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
				ImGui::Text("camera.id: %s", cameraComponent.id);
			}

			if (entity.hasComponent<ColorComponent>()) {
				const auto& colorComponent{ entity.getComponent<ColorComponent>() };
				displayVec4("Color.color: <%f , %f , %f , %f>", colorComponent.color);
			}

			if (entity.hasComponent<Texture2DComponent>()) {
				const auto& textureComponent{ entity.getComponent<Texture2DComponent>() };
				ImGui::Text("Texture2D.texturePath: %s", textureComponent.texturePath);
			}

			if (entity.hasComponent<TextureCubemapComponent>()) {
				const auto& textureComponent{ entity.getComponent<TextureCubemapComponent>() };
				ImGui::Text("TextureCubemap.texturePath: %s", textureComponent.texturePath);
			}

			if (entity.hasComponent<PointLightComponent>()) {
				const auto& pointLightComponent{ entity.getComponent<PointLightComponent>() };
				displayVec4("PointLight.position: <%f , %f , %f , %f>", pointLightComponent.pointLight.position);
				displayVec4("PointLight.ambient: <%f , %f , %f , %f>", pointLightComponent.pointLight.ambient);
				displayVec4("PointLight.diffuse: <%f , %f , %f , %f>", pointLightComponent.pointLight.diffuse);
				displayVec4("PointLight.specular: <%f , %f , %f , %f>", pointLightComponent.pointLight.specular);
				ImGui::Text("PointLight.constant: %f", pointLightComponent.pointLight.constant);
				ImGui::Text("PointLight.linear: %f", pointLightComponent.pointLight.linear);
				ImGui::Text("PointLight.quadratic: %f", pointLightComponent.pointLight.quadratic);
				ImGui::Text("PointLight.shininess: %f", pointLightComponent.pointLight.shininess);
			}


			ImGui::TreePop();
		}
	}

	void WSceneDebugWidget::displayMat4Transform(const char* name, const maths::mat4& transform) const {
		ImGui::Begin(name);

		ImGui::Text("--- %s ---", name);
		
		for (size_t i = 0; i < 4; i++) {
			ImGui::Text("%f %f %f %f", transform[0 + i * 4], transform[1 + i * 4], transform[2 + i * 4], transform[3 + i * 4]);
		}

		ImGui::Text("--- %s ---", name);

		ImGui::End();
	}


}
