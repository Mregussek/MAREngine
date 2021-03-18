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


#include "InspectorImGuiWidget.h"
#include "CommonTypeHandler.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../Events/EventsEntityImGuiWidget.h"
#include "Window/IWindow.h" // isMousePressed()
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Entity/EventsComponentEntity.h" // component add/update/remove events
#include "../../../../Core/graphics/Mesh/MeshCreator.h" // for correct renderable load
#include "../../../../Platform/OpenGL/TextureOpenGL.h"


namespace marengine {


    void FInspectorImGuiWidget::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pWindow = serviceLocator->retrieve<FImGuiTypeHolder<IWindow*>>()->pInstance;
    }

    void FInspectorImGuiWidget::resetInspectedEntity() {
        m_inspectedEntity = nullptr;
    }

    void FInspectorImGuiWidget::setEntityToInspect(const Entity& entityToInspect){
        m_inspectedEntity = &entityToInspect;
    }

    const Entity& FInspectorImGuiWidget::getInspectedEntity() const {
        return *m_inspectedEntity;
    }

    bool FInspectorImGuiWidget::isInspectedEntityValid() const {
        return m_inspectedEntity != nullptr;
    }

    void FInspectorImGuiWidget::updateFrame() {
        ImGui::Begin("Inspector Entity");

        if(!isInspectedEntityValid()) {
            ImGui::Text("No entity selected!");
        }
        else if (m_pSceneManagerEditor->isEditorMode()){
            displayEditorMode();
        }
        else if (m_pSceneManagerEditor->isPlayMode()) {
            displayPlayMode();
        }
        else {
            ImGui::Text("UnknownOption!");
        }

        handleInputs();

        ImGui::End();
    }

    void FInspectorImGuiWidget::displayEditorMode() {
        handle<TagComponent>("TagComponent");
        handle<TransformComponent>("TransformComponent");
        handle<PythonScriptComponent>("PythonScriptComponent");
        handle<RenderableComponent>("RenderableComponent");
        handle<CameraComponent>("CameraComponent");
        handle<ColorComponent>("ColorComponent");
        handle<Texture2DComponent>("Texture2DComponent");
        handle<TextureCubemapComponent>("TextureCubemapComponent");
        handle<PointLightComponent>("PointLightComponent");

        popUpMenu();
    }

    void FInspectorImGuiWidget::displayPlayMode() {
        ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

        handle<TransformComponent>("TransformComponent");
        handle<CameraComponent>("CameraComponent");
        handle<ColorComponent>("ColorComponent");
        handle<PointLightComponent>("PointLightComponent");
    }

    void FInspectorImGuiWidget::handleInputs() {
        if (m_newScriptWindow) {
            // TODO: add support for create and assign script to entity
            //WEntityFilesystemWidgets::Instance->openCreateAndAssignPythonScriptWidget();
            //m_newScriptWindow = false;
        }
        if (m_assignScriptWindow) {
            // TODO: add support for assign script to entity
            //WEntityFilesystemWidgets::Instance->openAssignPythonScriptWidget();
            //m_assignScriptWindow = false;
        }
    }

    void FInspectorImGuiWidget::popUpMenu() const {
        constexpr char popMenuName[]{ "SceneEntityModifyPopUp" };

        if (ImGui::IsWindowFocused() && m_pWindow->isMousePressed(MAR_MOUSE_BUTTON_2)) {
            ImGui::OpenPopup(popMenuName);
        }

        // Actual PopUp menu
        if (ImGui::BeginPopup(popMenuName)) {
            if (ImGui::BeginMenu("Children")) {
                displayChildrenPopMenu();
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Components")) {
                displayComponentPopMenu();
                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }
    }

    void FInspectorImGuiWidget::displayChildrenPopMenu() const {
        if (ImGui::MenuItem("Create and assign child")) {
            FEventsEntityImGuiWidgets::onCreateChild(getInspectedEntity());
        }

        if (ImGui::MenuItem("Assign new child")) {
            // TODO: add assignChildToEntity event
        }
    }

    void FInspectorImGuiWidget::displayComponentPopMenu() const {
        const bool hasRenderable{ m_inspectedEntity->hasComponent<RenderableComponent>() };
        const bool hasLight{ m_inspectedEntity->hasComponent<PointLightComponent>() };
        const bool hasCamera{ m_inspectedEntity->hasComponent<CameraComponent>() };
        const bool hasScript{ m_inspectedEntity->hasComponent<PythonScriptComponent>() };
        const bool hasNeitherColorNorTexture{
                !m_inspectedEntity->hasComponent<ColorComponent>()
                && !m_inspectedEntity->hasComponent<Texture2DComponent>()
                && !m_inspectedEntity->hasComponent<TextureCubemapComponent>()
        };

        if (!hasRenderable && ImGui::MenuItem("Add RenderableComponent")) {
            FEventsComponentEntity::onAdd<RenderableComponent>(getInspectedEntity());
        }

        if (hasNeitherColorNorTexture) {
            if (ImGui::MenuItem("Add ColorComponent")) {
                FEventsComponentEntity::onAdd<ColorComponent>(getInspectedEntity());
            }

            if (ImGui::MenuItem("Add Texture2DComponent")) {
                FEventsComponentEntity::onAdd<Texture2DComponent>(getInspectedEntity());
            }

            if (ImGui::MenuItem("Add TextureCubemapComponent")) {
                FEventsComponentEntity::onAdd<TextureCubemapComponent>(getInspectedEntity());
            }
        }

        if (!hasLight && ImGui::MenuItem("Add PointLightComponent")) {
            FEventsComponentEntity::onAdd<PointLightComponent>(getInspectedEntity());
        }

        if (!hasCamera && ImGui::MenuItem("Add CameraComponent")) {
            FEventsComponentEntity::onAdd<CameraComponent>(getInspectedEntity());
        }

        if (!hasScript && ImGui::MenuItem("Add PythonScriptComponent")) {
            FEventsComponentEntity::onAdd<PythonScriptComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<TagComponent>() {
        auto& tagComponent{ m_inspectedEntity->getComponent<TagComponent>() };
        FCommonTypeHandler::drawStringInputPanel<70>(tagComponent.tag);
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<TransformComponent>() {
        TransformComponent& tran{ m_inspectedEntity->getComponent<TransformComponent>() };

        const bool updatedTransform = [&tran]()->bool {
            const bool updatedPosition{
                    FCommonTypeHandler::drawVectorInputPanel("Position", tran.position, 0.f, 100.f, -10000.f, 10000.f)
            };
            const bool updatedRotation{
                    FCommonTypeHandler::drawVectorInputPanel("Rotation", tran.rotation, 0.f, 100.f, 0.f, 10.f)
            };
            const bool updatedScale{
                    FCommonTypeHandler::drawVectorInputPanel("Scale", tran.scale, 0.f, 100.f, 0.1f, 200.f)
            };

            if (updatedPosition || updatedRotation || updatedScale) {
                return true;
            }

            return false;
        }();

        ImGui::NewLine();
        if (updatedTransform) {
            FEventsComponentEntity::onUpdate<TransformComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<PythonScriptComponent>() {
        if (ImGui::MenuItem("Remove Script")) {
            FEventsComponentEntity::onRemove<PythonScriptComponent>(getInspectedEntity());
            return;
        }

        PythonScriptComponent& pythonScriptComponent{ m_inspectedEntity->getComponent<PythonScriptComponent>() };
        ImGui::Text("Current script: %s", pythonScriptComponent.scriptsPath.c_str());

        if (ImGui::Button("*** Open in Script Editor")) {
            // TODO: add open in script editor option
            //std::string sourceCode;
            //FFileManager::loadFile(sourceCode, pythonScriptComponent.scriptsPath.c_str());
            // TODO! restore filename from path
            //const std::string restoredFilename{ pythonScriptComponent.scriptsPath };
            //WScriptIDE::Instance->setEditorTitle(restoredFilename);
            //WScriptIDE::Instance->setEditorCode(sourceCode);
            //WScriptIDE::Instance->setPathToScript(pythonScriptComponent.scriptsPath);
        }
        if (ImGui::Button("*** Create new file and assign it as script")) {
            m_newScriptWindow = true;
        }
        if (ImGui::Button("*** Assign existing script to entity")) {
            m_assignScriptWindow = true;
        }
    }

    template<typename T>
    bool Button_ChooseRenderable(RenderableComponent& renderable, const char* buttonName) {
        if (ImGui::Button(buttonName)) {
            renderable.name = T::getID();
            renderable.vertices = T::getVertices();
            renderable.indices = T::getIndices();
            return true;
        }

        return false;
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<RenderableComponent>() {
        if (ImGui::MenuItem("Remove Renderable")) {
            FEventsComponentEntity::onRemove<RenderableComponent>(getInspectedEntity());
            return;
        }

        RenderableComponent& renderable{ m_inspectedEntity->getComponent<RenderableComponent>() };
        ImGui::Text("Type: %s", renderable.name.c_str());

        const bool userHasChosenRenderable = [&renderable]()->bool {
            if (Button_ChooseRenderable<MeshCreator::Cube>(renderable, "Cube")) { return true; }
            ImGui::SameLine();
            if (Button_ChooseRenderable<MeshCreator::Pyramid>(renderable, "Pyramid")) { return true; }
            ImGui::SameLine();
            if (Button_ChooseRenderable<MeshCreator::Wall>(renderable, "Wall")) { return true; }
            ImGui::SameLine();
            if (Button_ChooseRenderable<MeshCreator::Surface>(renderable, "Surface")) { return true; }

            return false;
        }();

        if (userHasChosenRenderable) {
            FEventsComponentEntity::onUpdate<RenderableComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CameraComponent>() {
        if (ImGui::Button("Remove Camera")) {
            FEventsComponentEntity::onRemove<CameraComponent>(getInspectedEntity());
            return;
        }
        CameraComponent& camera{ m_inspectedEntity->getComponent<CameraComponent>() };

        ImGui::Text("WARNING: To use camera in PlayMode please set Camera ID to \"main\"!");

        FCommonTypeHandler::drawStringInputPanel<50>(camera.id);

        bool updatedCamera = false;

        if(ImGui::Checkbox("Perspective (True) / Orthographic (False)", &camera.Perspective)) {
            updatedCamera = true;
        }
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
            FEventsComponentEntity::onUpdate<CameraComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<ColorComponent>() {
        if (ImGui::MenuItem("Remove Color")) {
            FEventsComponentEntity::onRemove<ColorComponent>(getInspectedEntity());
            return;
        }
        ColorComponent& color{ m_inspectedEntity->getComponent<ColorComponent>() };

        if (ImGui::ColorEdit4("- color", &color.color.x)) {
            FEventsComponentEntity::onUpdate<ColorComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<Texture2DComponent>() {
        if (ImGui::MenuItem("Remove Texture")) {
            FEventsComponentEntity::onRemove<Texture2DComponent>(getInspectedEntity());
            return;
        }
        Texture2DComponent& texture2D{ m_inspectedEntity->getComponent<Texture2DComponent>() };

        ImGui::Text("Current Texture: %s", texture2D.texturePath.c_str());

        if (TextureOpenGL::hasTexture(texture2D.texturePath)) {
            ImGui::Image((ImTextureID)TextureOpenGL::getTexture(texture2D.texturePath), { 100.f, 100.f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        }

        const bool changedTexture{ FCommonTypeHandler::drawStringInputPanel<128>(texture2D.texturePath) };
        if(changedTexture && TextureOpenGL::hasTexture(texture2D.texturePath)) {
            FEventsComponentEntity::onUpdate<Texture2DComponent>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<PointLightComponent>() {
        if (ImGui::MenuItem("Remove Light")) {
            FEventsComponentEntity::onRemove<PointLightComponent>(getInspectedEntity());
            return;
        }
        FPointLight& pointLight{ m_inspectedEntity->getComponent<PointLightComponent>().pointLight };
        bool updatedLight = false;

        if (FCommonTypeHandler::drawVectorInputPanel("Ambient", pointLight.ambient, 0.f, 100.f, 0.01f, 100.f)) { updatedLight = true; }
        if (FCommonTypeHandler::drawVectorInputPanel("Diffuse", pointLight.diffuse, 0.f, 100.f, 0.01f, 100.f)) { updatedLight = true; }
        if (FCommonTypeHandler::drawVectorInputPanel("Specular", pointLight.specular, 0.f, 100.f, 0.01f, 100.f)) { updatedLight = true; }

        ImGui::NewLine();

        if (ImGui::DragFloat("Constant", &pointLight.constant, 0.001f, 0.f, 2.f)) { updatedLight = true; }
        if (ImGui::DragFloat("Linear", &pointLight.linear, 0.001f, 0.f, 0.5f)) { updatedLight = true; }
        if (ImGui::DragFloat("Quadratic", &pointLight.quadratic, 0.001f, 0.f, 0.1f)) { updatedLight = true; }
        if (ImGui::DragFloat("Shininess", &pointLight.shininess, 0.5f, 0.f, 256.f)) { updatedLight = true; }

        if (updatedLight) {
            FEventsComponentEntity::onUpdate<PointLightComponent>(getInspectedEntity());
        }
    }


}
