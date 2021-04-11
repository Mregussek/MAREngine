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
#include "../../../../Platform/OpenGL/TextureOpenGL.h"


namespace marengine {


    void FInspectorImGuiWidget::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pSceneManagerEditor = serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pWindow = serviceLocator->retrieve<FImGuiTypeHolder<FWindow*>>()->pInstance;
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
        handle<CTag>("CTag");
        handle<CTransform>("CTransform");
        handle<CPythonScript>("CPythonScript");
        handle<CRenderable>("CRenderable");
        handle<CCamera>("CCamera");
        handle<CPointLight>("CPointLight");

        popUpMenu();
    }

    void FInspectorImGuiWidget::displayPlayMode() {
        ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

        handle<CTransform>("CTransform");
        handle<CCamera>("CCamera");
        handle<CPointLight>("CPointLight");
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
        const bool hasRenderable{ m_inspectedEntity->hasComponent<CRenderable>() };
        const bool hasLight{ m_inspectedEntity->hasComponent<CPointLight>() };
        const bool hasCamera{ m_inspectedEntity->hasComponent<CCamera>() };
        const bool hasScript{ m_inspectedEntity->hasComponent<CPythonScript>() };

        if (!hasRenderable && ImGui::MenuItem("Add CRenderable")) {
            FEventsComponentEntity::onAdd<CRenderable>(getInspectedEntity());
        }

        // TODO: return back material usage
        //const bool hasNeitherColorNorTexture{
        //        !m_inspectedEntity->hasComponent<ColorComponent>()
        //        && !m_inspectedEntity->hasComponent<Texture2DComponent>()
        //        && !m_inspectedEntity->hasComponent<TextureCubemapComponent>()
        //};
        //if (hasNeitherColorNorTexture) {
        //    if (ImGui::MenuItem("Add ColorComponent")) {
        //        FEventsComponentEntity::onAdd<ColorComponent>(getInspectedEntity());
        //    }
        //    if (ImGui::MenuItem("Add Texture2DComponent")) {
        //        FEventsComponentEntity::onAdd<Texture2DComponent>(getInspectedEntity());
        //    }
        //    if (ImGui::MenuItem("Add TextureCubemapComponent")) {
        //        FEventsComponentEntity::onAdd<TextureCubemapComponent>(getInspectedEntity());
        //    }
        //}

        if (!hasLight && ImGui::MenuItem("Add CPointLight")) {
            FEventsComponentEntity::onAdd<CPointLight>(getInspectedEntity());
        }

        if (!hasCamera && ImGui::MenuItem("Add CCamera")) {
            FEventsComponentEntity::onAdd<CCamera>(getInspectedEntity());
        }

        if (!hasScript && ImGui::MenuItem("Add CPythonScript")) {
            FEventsComponentEntity::onAdd<CPythonScript>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CTag>() {
        auto& tagComponent{ m_inspectedEntity->getComponent<CTag>() };
        FCommonTypeHandler::drawStringInputPanel<70>(tagComponent.tag);
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CTransform>() {
        CTransform& tran{ m_inspectedEntity->getComponent<CTransform>() };

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
            FEventsComponentEntity::onUpdate<CTransform>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CPythonScript>() {
        if (ImGui::MenuItem("Remove Script")) {
            FEventsComponentEntity::onRemove<CPythonScript>(getInspectedEntity());
            return;
        }

        auto& cPythonScript{ m_inspectedEntity->getComponent<CPythonScript>() };
        ImGui::Text("Current script: %s", cPythonScript.scriptsPath.c_str());

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

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CRenderable>() {
        if (ImGui::MenuItem("Remove Renderable")) {
            FEventsComponentEntity::onRemove<CRenderable>(getInspectedEntity());
            return;
        }

        // TODO: return back renderable component on inspector
        //CRenderable& renderable{ m_inspectedEntity->getComponent<CRenderable>() };
        //ImGui::Text("Type: %s", renderable.name.c_str());

        //const bool userHasChosenRenderable = [&renderable]()->bool {
        //    if (Button_ChooseRenderable<MeshCreator::Cube>(renderable, "Cube")) { return true; }
        //    ImGui::SameLine();
        //    if (Button_ChooseRenderable<MeshCreator::Pyramid>(renderable, "Pyramid")) { return true; }
        //    ImGui::SameLine();
        //    if (Button_ChooseRenderable<MeshCreator::Wall>(renderable, "Wall")) { return true; }
        //    ImGui::SameLine();
        //    if (Button_ChooseRenderable<MeshCreator::Surface>(renderable, "Surface")) { return true; }
        //    return false;
        //}();

        //if (userHasChosenRenderable) {
        //    FEventsComponentEntity::onUpdate<CRenderable>(getInspectedEntity());
        //}
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CCamera>() {
        if (ImGui::Button("Remove Camera")) {
            FEventsComponentEntity::onRemove<CCamera>(getInspectedEntity());
            return;
        }
        CCamera& camera{ m_inspectedEntity->getComponent<CCamera>() };

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
            FEventsComponentEntity::onUpdate<CCamera>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorImGuiWidget::displayComponentPanel<CPointLight>() {
        if (ImGui::MenuItem("Remove Light")) {
            FEventsComponentEntity::onRemove<CPointLight>(getInspectedEntity());
            return;
        }
        FPointLight& pointLight{ m_inspectedEntity->getComponent<CPointLight>().pointLight };
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
            FEventsComponentEntity::onUpdate<CPointLight>(getInspectedEntity());
        }
    }


}
