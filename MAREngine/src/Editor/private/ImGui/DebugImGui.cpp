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


#include "DebugImGui.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../../ProjectManager.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/graphics/public/Renderer.h"


namespace marengine {


    void FDebugWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pRenderStatistics =
                pServiceLocator->retrieve<FHolderPtr<FRenderStatistics*>>()->pInstance;
    }

    void FDebugWidgetImGui::updateFrame() {
        Scene* pScene{ m_pSceneManagerEditor->getScene() };
        ImGui::Begin("SceneDebugWidget");

        const FProjectInfo& projectInfo{ FProjectManager::getProjectInfo() };

        ImGui::Text("absolutePath: %s", projectInfo.absolutePath.c_str());
        ImGui::Text("projectName: %s", projectInfo.projectName.c_str());
        ImGui::Text("projectPath: %s", projectInfo.projectPath.c_str());
        ImGui::Text("assetsPath: %s", projectInfo.assetsPath.c_str());
        ImGui::Text("scenesPath: %s", projectInfo.scenesPath.c_str());
        ImGui::Text("sceneToLoadAtStartup: %s", projectInfo.sceneToLoadAtStartup.c_str());
        ImGui::Text("windowName: %s", projectInfo.windowName.c_str());

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

    void FDebugWidgetImGui::displayInfoAbout(Scene* pScene) const {
        ImGui::Text("SceneName: %s", pScene->getName().c_str());

        const maths::vec3& background{ pScene->getBackground() };
        ImGui::Text("SceneBackground: (%f , %f , %f)", background.x, background.y, background.z);
    }

    void FDebugWidgetImGui::displayInfoAbout(const Entity& entity) const {
        if (ImGui::TreeNode(entity.getComponent<CTag>().tag.c_str())) {
            const auto displayVec3 = [](const char* label, const maths::vec3& v) {
                ImGui::Text(label, v.x, v.y, v.z);
            };
            const auto displayVec4 = [](const char* label, const maths::vec4& v) {
                ImGui::Text(label, v.x, v.y, v.z, v.w);
            };

            ImGui::Text("hasChildren: %i", entity.hasChildren());
            ImGui::Text("ChildrenCount: %i", entity.getChildren().size());

            const auto& transformComponent{ entity.getComponent<CTransform>() };
            displayVec3("Transform.center: <%f , %f , %f>", transformComponent.position);
            displayVec3("Transform.rotation: <%f , %f , %f>", transformComponent.rotation);
            displayVec3("Transform.scale: <%f , %f , %f>", transformComponent.scale);

            if (entity.hasComponent<CPythonScript>()) {
                const auto& cPythonScript{ entity.getComponent<CPythonScript>() };
                ImGui::Text("cPythonScript.scriptsPath: %s", cPythonScript.scriptsPath.c_str());
            }

            if(entity.hasComponent<CRenderable>()){
                const auto& cRenderable{ entity.getComponent<CRenderable>() };
                ImGui::Text("cRenderable.mesh.index: %i", cRenderable.mesh.index);
                ImGui::Text("cRenderable.mesh.type: %i", cRenderable.mesh.type);
                ImGui::Text("cRenderable.batch.index: %i", cRenderable.batch.index);
                ImGui::Text("cRenderable.batch.transformIndex: %i", cRenderable.batch.transformIndex);
                ImGui::Text("cRenderable.batch.materialIndex: %i", cRenderable.batch.materialIndex);
                ImGui::Text("cRenderable.material.index: %i", cRenderable.material.index);
                // TODO: add material type in debug window
                displayVec4("cRenderable.color: <%f , %f , %f , %f>", cRenderable.color);
            }

            if (entity.hasComponent<CCamera>()) {
                const auto& cameraComponent{ entity.getComponent<CCamera>() };
                ImGui::Text("camera.id: %s", cameraComponent.id.c_str());
            }

            if (entity.hasComponent<CPointLight>()) {
                const auto& pointLight{ entity.getComponent<CPointLight>().pointLight };
                const auto& batchInfo{ entity.getComponent<CPointLight>().batch };
                ImGui::Text("CPointLight.batch.index: %i", batchInfo.index);
                ImGui::Text("CPointLight.batch.type: %i", batchInfo.type);
                displayVec4("CPointLight.pointLight.position: <%f , %f , %f , %f>", pointLight.position);
                displayVec4("CPointLight.pointLight.ambient: <%f , %f , %f , %f>", pointLight.ambient);
                displayVec4("CPointLight.pointLight.diffuse: <%f , %f , %f , %f>", pointLight.diffuse);
                displayVec4("CPointLight.pointLight.specular: <%f , %f , %f , %f>", pointLight.specular);
                ImGui::Text("CPointLight.pointLight.constant: %f", pointLight.constant);
                ImGui::Text("CPointLight.pointLight.linear: %f", pointLight.linear);
                ImGui::Text("CPointLight.pointLight.quadratic: %f", pointLight.quadratic);
                ImGui::Text("CPointLight.pointLight.shininess: %f", pointLight.shininess);
            }

            ImGui::TreePop();
        }
    }


}
