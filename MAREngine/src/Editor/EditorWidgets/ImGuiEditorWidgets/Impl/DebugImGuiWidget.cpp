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


#include "DebugImGuiWidget.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/graphics/Renderer.h"


namespace marengine {


    void FDebugImGuiWidget::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pRenderStatistics = serviceLocator->retrieve<FImGuiTypeHolder<FRenderStatistics*>>()->pInstance;
    }

    void FDebugImGuiWidget::updateFrame() {
        Scene* pScene{ m_pSceneManagerEditor->getScene() };
        ImGui::Begin("SceneDebugWidget");

        displayInfoAbout(pScene);

        ImGui::Separator();

        const FEntityArray& entities{ pScene->getEntities() };
        for (const Entity& entity : entities) {
            displayInfoAbout(entity);
            ImGui::Separator();
        }

        ImGui::End();
        ImGui::Begin("Statistics Menu");

        m_pRenderStatistics->update(m_pSceneManagerEditor);
        const FRenderStatsStorage& storage{ m_pRenderStatistics->getStorage() };

        ImGui::Text("Draw Calls: %d", storage.drawCallsCount);
        ImGui::Text("Vertices: %d", storage.verticesCount);
        ImGui::Text("Indices: %d", storage.indicesCount);
        ImGui::Text("Triangles: %d", storage.trianglesCount);
        ImGui::Text("Entities: %d", storage.entitiesCount);
        ImGui::Text("Colored Entities: %d", storage.coloredEntitiesCount);
        ImGui::Text("Textured2D Entities: %d", storage.textured2dEntitiesCount);
        ImGui::Text("Rendered Entities: %d", storage.allRendererEntitiesCount);

        ImGui::Separator();

        const float framerate{  ImGui::GetIO().Framerate };
        ImGui::Text("FPS: %f", framerate);
        ImGui::Text("ms/frame: %f", 1000.0f / framerate);

        ImGui::End();
    }

    void FDebugImGuiWidget::displayInfoAbout(Scene* pScene) const {
        ImGui::Text("SceneName: %s", pScene->getName().c_str());

        const maths::vec3& background{ pScene->getBackground() };
        ImGui::Text("SceneBackground: (%f , %f , %f)", background.x, background.y, background.z);
    }

    void FDebugImGuiWidget::displayInfoAbout(const Entity& entity) const {
        if (ImGui::TreeNode(entity.getComponent<TagComponent>().tag.c_str())) {
            const auto displayVec3 = [](const char* label, const maths::vec3& v) {
                ImGui::Text(label, v.x, v.y, v.z);
            };
            const auto displayVec4 = [](const char* label, const maths::vec4& v) {
                ImGui::Text(label, v.x, v.y, v.z, v.w);
            };

            ImGui::Text("hasChildren: %i", entity.hasChildren());
            ImGui::Text("ChildrenCount: %i", entity.getChildren().size());

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

            // TODO: add renderablecomponent to debug widget
            //if (entity.hasComponent<RenderableComponent>()) {
            //    const auto& renderableComponent{ entity.getComponent<RenderableComponent>() };
            //    ImGui::Text("Renderable.name: %s", renderableComponent.name.c_str());
            //    ImGui::Text("Renderable.vertices.size: %i", renderableComponent.vertices.size());
            //    ImGui::Text("Renderable.indices.size: %i", renderableComponent.indices.size());
            //}

            if (entity.hasComponent<CameraComponent>()) {
                const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
                ImGui::Text("camera.id: %s", cameraComponent.id.c_str());
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


}
