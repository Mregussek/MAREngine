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


#include "InspectorImGui.h"
#include "CommonTypeHandler.h"
#include "ScriptImGui.h"
#include "FilesystemPopUpImGui.h"
#include "ContentBrowserImGui.h"
#include "../../public/EventsComponentEditor.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../public/EventsEntityEditor.h"
#include "../../../Window/IWindow.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/graphics/public/MaterialManager.h"


namespace marengine {


    void FInspectorWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pFilesystem = pServiceLocator->retrieve<FFilesystemPopUpImGuiWidget>();
        m_pContentBrowser = pServiceLocator->retrieve<FContentBrowserImGui>();
        m_pScriptWidget = pServiceLocator->retrieve<FScriptWidgetImGui>();
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pWindow =
                pServiceLocator->retrieve<FHolderPtr<FWindow*>>()->pInstance;
    }

    void FInspectorWidgetImGui::updateFrame() {
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

        ImGui::End();
    }

    void FInspectorWidgetImGui::displayEditorMode() {
        handle<CTag>("CTag");
        handle<CTransform>("CTransform");
        handle<CPythonScript>("CPythonScript");
        handle<CRenderable>("CRenderable");
        handle<CCamera>("CCamera");
        handle<CPointLight>("CPointLight");

        popUpMenu();
    }

    void FInspectorWidgetImGui::displayPlayMode() {
        ImGui::Text("Cannot modify entity parameters during play mode other than:\n\tTransform, Camera, Light, Color");

        handle<CTransform>("CTransform");
        handle<CCamera>("CCamera");
        handle<CPointLight>("CPointLight");
    }

    void FInspectorWidgetImGui::popUpMenu() const {
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

    void FInspectorWidgetImGui::displayChildrenPopMenu() const {
        if (ImGui::MenuItem("Create and assign child")) {
            FEventsEntityEditor::onCreateChild(getInspectedEntity());
        }

        if (ImGui::MenuItem("Assign new child")) {
            // TODO: add assignChildToEntity event
        }
    }

    void FInspectorWidgetImGui::displayComponentPopMenu() const {
        const bool hasRenderable{ p_pInspectedEntity->hasComponent<CRenderable>() };
        const bool hasLight{ p_pInspectedEntity->hasComponent<CPointLight>() };
        const bool hasCamera{ p_pInspectedEntity->hasComponent<CCamera>() };
        const bool hasScript{ p_pInspectedEntity->hasComponent<CPythonScript>() };

        if (!hasRenderable && ImGui::MenuItem("Add CRenderable")) {
            FEventsComponentEditor::onAdd<CRenderable>(getInspectedEntity());
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
            FEventsComponentEditor::onAdd<CPointLight>(getInspectedEntity());
        }

        if (!hasCamera && ImGui::MenuItem("Add CCamera")) {
            FEventsComponentEditor::onAdd<CCamera>(getInspectedEntity());
        }

        if (!hasScript && ImGui::MenuItem("Add CPythonScript")) {
            FEventsComponentEditor::onAdd<CPythonScript>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CTag>() {
        auto& tagComponent{ p_pInspectedEntity->getComponent<CTag>() };
        FCommonTypeHandler::drawStringInputPanel<70>(tagComponent.tag);
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CTransform>() {
        CTransform& tran{ p_pInspectedEntity->getComponent<CTransform>() };

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
            FEventsComponentEditor::onUpdate<CTransform>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CPythonScript>() {
        // During CPythonScripts onUpdate events, we need to create CEvent component and fill with
        // actual event.

        const std::string newScriptWindow{ "Create New Script and assign it" };
        const std::string assignScriptWindow{ "Assign Script" };
        const std::string pyExt{ ".py" };
        constexpr char openScriptButton[]{ "*** Open in Script Editor" };
        constexpr char createNewButton[]{ "*** Create new file and assign it as script" };
        constexpr char assignExistButton[]{ "*** Assign existing script to entity" };

        if (ImGui::MenuItem("Remove Script")) {
            FEventsComponentEditor::onRemove<CPythonScript>(getInspectedEntity());
            return;
        }

        auto& cPythonScript{ p_pInspectedEntity->getComponent<CPythonScript>() };
        ImGui::Text("Current script: %s", cPythonScript.scriptsPath.c_str());

        if (ImGui::Button(openScriptButton)) {
            p_pInspectedEntity->addComponent<CEvent>(EEventType::PYTHONSCRIPT_OPEN);
            FEventsComponentEditor::onUpdate<CPythonScript>(getInspectedEntity());
            p_pInspectedEntity->removeComponent<CEvent>();

        }
        if (ImGui::Button(createNewButton)) {
            m_newScriptWindow = true;
        }
        if (ImGui::Button(assignExistButton)) {
            m_assignScriptWindow = true;
        }

        if(m_newScriptWindow) {
            m_pFilesystem->openWidget(newScriptWindow);
            m_newScriptWindow = false;
        }
        if(m_assignScriptWindow) {
            m_pFilesystem->openWidget(assignScriptWindow);
            m_assignScriptWindow = false;
        }

        const FFilesystemDialogInfo newDialogInfo =
                m_pFilesystem->displaySaveWidget(newScriptWindow, pyExt);
        if(newDialogInfo.isValid()) {
            p_pInspectedEntity->addComponent<CEvent>(EEventType::PYTHONSCRIPT_CREATE_ASSIGN,
                                                     &newDialogInfo);
            FEventsComponentEditor::onUpdate<CPythonScript>(getInspectedEntity());
            p_pInspectedEntity->removeComponent<CEvent>();
        }

        const FFilesystemDialogInfo assignDialogInfo =
                m_pFilesystem->displayOpenWidget(assignScriptWindow, pyExt);
        if(assignDialogInfo.isValid()) {
            p_pInspectedEntity->addComponent<CEvent>(EEventType::PYTHONSCRIPT_ASSIGN,
                                                     &newDialogInfo);
            FEventsComponentEditor::onUpdate<CPythonScript>(getInspectedEntity());
            p_pInspectedEntity->removeComponent<CEvent>();
        }
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CRenderable>() {
        // During CRenderable onUpdate events, we need to create CEvent component and fill with
        // actual event.
        const std::string loadTexture2D{ "Load Texture 2D" };
        const std::string jpgExt{ ".jpg" };
        constexpr char loadTexture2DButton[]{ "*** Load Texture 2D" };

        if (ImGui::MenuItem("Remove Renderable")) {
            FEventsComponentEditor::onRemove<CRenderable>(getInspectedEntity());
            return;
        }

        CRenderable& cRenderable{ p_pInspectedEntity->getComponent<CRenderable>() };
        ImGui::Text("Current: %s", cRenderable.mesh.path.c_str());

        if(!cRenderable.material.isValid()) { // we are displaying color then
            const bool changedColor = ImGui::ColorEdit4("Color", &cRenderable.color.x);
            if(changedColor) {
                p_pInspectedEntity->addComponent<CEvent>(EEventType::RENDERABLE_COLOR_UPDATE);
                FEventsComponentEditor::onUpdate<CRenderable>(getInspectedEntity());
                p_pInspectedEntity->removeComponent<CEvent>();
            }

            if (!cRenderable.material.isValid() && ImGui::Button(loadTexture2DButton)) {
                m_loadTex2D = true;
            }
        }
        else { // managing specific material

        }

        const bool selectedMesh{ m_pContentBrowser->drawMeshListBox(cRenderable) };
        if(selectedMesh) {
            p_pInspectedEntity->addComponent<CEvent>(EEventType::RENDERABLE_MESH_UPDATE);
            FEventsComponentEditor::onUpdate<CRenderable>(getInspectedEntity());
            p_pInspectedEntity->removeComponent<CEvent>();
        }
        if(m_loadTex2D) {
            m_pFilesystem->openWidget(loadTexture2D);
            m_loadTex2D = false;
        }
        const FFilesystemDialogInfo loadTex2DInfo =
                m_pFilesystem->displayOpenWidget(loadTexture2D, jpgExt);
        if(loadTex2DInfo.isValid()) {
            p_pInspectedEntity->addComponent<CEvent>(EEventType::RENDERABLE_TEX2D_LOAD,
                                                     &loadTex2DInfo);
            FEventsComponentEditor::onUpdate<CRenderable>(getInspectedEntity());
            p_pInspectedEntity->removeComponent<CEvent>();
        }
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CCamera>() {
        if (ImGui::Button("Remove Camera")) {
            FEventsComponentEditor::onRemove<CCamera>(getInspectedEntity());
            return;
        }
        CCamera& camera{ p_pInspectedEntity->getComponent<CCamera>() };

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
            FEventsComponentEditor::onUpdate<CCamera>(getInspectedEntity());
        }
    }

    template<>
    void FInspectorWidgetImGui::displayComponentPanel<CPointLight>() {
        if (ImGui::MenuItem("Remove Light")) {
            FEventsComponentEditor::onRemove<CPointLight>(getInspectedEntity());
            return;
        }
        FPointLight& pointLight{ p_pInspectedEntity->getComponent<CPointLight>().pointLight };
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
            FEventsComponentEditor::onUpdate<CPointLight>(getInspectedEntity());
        }
    }


}
