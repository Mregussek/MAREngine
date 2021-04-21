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


#include "ViewportWidgetImGui.h"
#include "InspectorWidgetImGui.h"
#include "../ImGuiEditorServiceLocator.h"
#include "../../../../Window/IWindow.h"
#include "../../../../Core/graphics/public/RenderManager.h"
#include "../../../../Core/graphics/public/Framebuffer.h"
#include "../../../../Core/ecs/SceneManagerEditor.h"
#include "../../../../Core/ecs/Scene.h"
#include "../../../../Core/ecs/Entity/Entity.h"
#include "../../../../Core/ecs/Entity/EventsCameraEntity.h"
#include "../../../../Core/ecs/Entity/EventsComponentEntity.h"


namespace marengine {

    static void drawGuizmo(ImGuizmo::OPERATION guizmoOperation, const Camera& editorCamera,
                           const Entity& currentEntity);


    void FViewportWidgetImGui::create(FImGuiEditorServiceLocator* serviceLocator) {
        m_pRenderManager =
                serviceLocator->retrieve<FImGuiTypeHolder<FRenderManager*>>()->pInstance;
        m_pSceneManagerEditor =
                serviceLocator->retrieve<FImGuiTypeHolder<FSceneManagerEditor*>>()->pInstance;
        m_pInspectorWidget = serviceLocator->retrieve<FInspectorWidgetImGui>();
        m_pWindow = serviceLocator->retrieve<FImGuiTypeHolder<FWindow*>>()->pInstance;

        FFramebuffer* pFramebuffer{ m_pRenderManager->getViewportFramebuffer() };
        const uint32 currentWidth{ pFramebuffer->getWidth() };
        const uint32 currentHeight{ pFramebuffer->getHeight() };
        m_aspectRatio = (float)currentWidth / (float)currentHeight;

        m_camera.initialize(m_aspectRatio);

        if(m_pSceneManagerEditor->usingEditorCamera()) {
            FEventsCameraEntity::onEditorCameraSet(m_camera.getCameraData());
        }
    }

    void FViewportWidgetImGui::updateFrame() {
        const ImGuizmo::OPERATION guizmoOperation{ displayViewportControlPanel() };

        ImGuiStyle& style{ ImGui::GetStyle() };
        const ImVec2 rememberDefaultVal{ style.WindowPadding };
        style.WindowPadding = ImVec2(1.f, 1.f);

        ImGui::Begin("ViewPort", nullptr);

        displayActualViewport();

        if (m_pSceneManagerEditor->usingEditorCamera() && m_pSceneManagerEditor->isEditorMode()) {
            if (m_pInspectorWidget->isInspectedEntityValid()) {
                drawGuizmo(guizmoOperation, m_camera, m_pInspectorWidget->getInspectedEntity());
            }

            const bool cameraWasRecalculated =
                    m_camera.update(m_pWindow, m_aspectRatio, ImGui::IsWindowFocused());
            if (cameraWasRecalculated) {
                FEventsCameraEntity::onEditorCameraSet(m_camera.getCameraData());
            }
        }

        ImGui::End();

        style.WindowPadding = rememberDefaultVal;
    }

    static void displayEditorModeButtons(FSceneManagerEditor* pSceneManagerEditor,
                                         const RenderCamera* pRenderCamera) {
        if (ImGui::Button("PLAY")) {
            pSceneManagerEditor->setPlayMode();
        }

        ImGui::SameLine();

        if (pSceneManagerEditor->usingEditorCamera()) {
            if(ImGui::Button("Use Game Camera")) {
                pSceneManagerEditor->useGameCamera();
                FEventsCameraEntity::onGameCameraSet();
            }
        }
        else {
            if (ImGui::Button("Use Editor Camera")) {
                pSceneManagerEditor->useEditorCamera();
                FEventsCameraEntity::onEditorCameraSet(pRenderCamera);
            }
        }
    }

    static void displayPlayModeButtons(FSceneManagerEditor* pSceneManagerEditor,
                                       const RenderCamera* pRenderCamera) {
        if (ImGui::Button("STOP")) {
            pSceneManagerEditor->setExitPlayMode();
            if (pSceneManagerEditor->usingEditorCamera()) {
                FEventsCameraEntity::onEditorCameraSet(pRenderCamera);
            }
        }
        ImGui::SameLine();
        if (pSceneManagerEditor->isPauseMode()) {
            if (ImGui::Button("RESUME")) {
                pSceneManagerEditor->setExitPauseMode();
            }
        }
        else {
            if (ImGui::Button("PAUSE")) {
                pSceneManagerEditor->setPauseMode();
            }
        }
    }

    ImGuizmo::OPERATION FViewportWidgetImGui::displayViewportControlPanel() {
        constexpr uint8 guizmoTypeCount{ 4 };
        enum GuizmoType { NONE, TRANSLATION, ROTATION, SCALE };
        static int32 guizmoIndex{ NONE };
        constexpr std::array<const char*, guizmoTypeCount> guizmoTypes{
            "None", "Translation", "Rotation", "Scale"
        };
        const char* currentGuizmoType =
                (guizmoIndex >= 0 && guizmoIndex < guizmoTypeCount) ? guizmoTypes[guizmoIndex] : "Unknown";

        ImGui::Begin("Viewport Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (m_pSceneManagerEditor->isEditorMode()) {
                displayEditorModeButtons(m_pSceneManagerEditor, m_camera.getCameraData());
            }
            else {
                displayPlayModeButtons(m_pSceneManagerEditor, m_camera.getCameraData());
            }

            ImGui::SliderInt("GuizmoType", &guizmoIndex, 0, guizmoTypeCount - 1, currentGuizmoType);

            ImGui::EndMenuBar();
        }

        ImGui::End();

        if (m_pWindow->isKeyPressed(MAR_KEY_LEFT_CONTROL)) {
            if (m_pWindow->isKeyPressed(MAR_KEY_Z)) { guizmoIndex = TRANSLATION; }
            if (m_pWindow->isKeyPressed(MAR_KEY_X)) { guizmoIndex = ROTATION; }
            if (m_pWindow->isKeyPressed(MAR_KEY_C)) { guizmoIndex = SCALE; }
            if (m_pWindow->isKeyPressed(MAR_KEY_V)) { guizmoIndex = NONE; }
        }

        switch(guizmoIndex) {
            case TRANSLATION: return ImGuizmo::OPERATION::TRANSLATE;
            case ROTATION: return ImGuizmo::OPERATION::ROTATE;
            case SCALE: return ImGuizmo::OPERATION::SCALE;
            default: return ImGuizmo::OPERATION::NONE;
        }
    }

    void FViewportWidgetImGui::displayActualViewport() {
        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        FFramebuffer* pFramebuffer{ m_pRenderManager->getViewportFramebuffer() };
        const uint32 currentWidth{ pFramebuffer->getWidth() };
        const uint32 currentHeight{ pFramebuffer->getHeight() };

        const bool viewportSizeMatch{
            currentWidth != (uint32)viewportSize.x || currentHeight != (uint32)viewportSize.y
        };
        if (viewportSizeMatch) {
            pFramebuffer->resize((uint32)viewportSize.x, (uint32)viewportSize.y);
            m_aspectRatio = (float)currentWidth / (float)currentHeight;
        }

        const uint32_t id{ pFramebuffer->getColorAttach() };
        ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }


    static bool draw(ImGuizmo::OPERATION guizmoOperation, const Camera& editorCamera,
                     CTransform& transformComponent);

    void drawGuizmo(ImGuizmo::OPERATION guizmoOperation, const Camera& editorCamera,
                    const Entity& currentEntity) {
        auto& transform = currentEntity.getComponent<CTransform>();
        if (guizmoOperation == ImGuizmo::OPERATION::NONE) {
            return;
        }

        const bool userUsedGuizmo{ draw(guizmoOperation, editorCamera, transform) };
        if (userUsedGuizmo) {
            FEventsComponentEntity::onUpdate<CTransform>(currentEntity);
        }
    }

    bool draw(ImGuizmo::OPERATION guizmoOperation, const Camera& editorCamera,
              CTransform& transformComponent) {
        using namespace maths;

        mat4 transform{ transformComponent.getTransform() };
        float* pTransform{ transform.value_ptr_nonconst() };

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        const ImVec2 windowPos{ ImGui::GetWindowPos() };
        const ImVec2 windowSize{ ImGui::GetWindowSize() };
        ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

        const float* pView{ nullptr };
        const float* pProjection{ nullptr };
        {
            const RenderCamera* pRenderCam{ editorCamera.getCameraData() };
            pView = pRenderCam->getView().value_ptr();
            pProjection = pRenderCam->getProjection().value_ptr();
        }

        ImGuizmo::Manipulate(pView, pProjection, guizmoOperation, ImGuizmo::MODE::LOCAL, pTransform);

        if (ImGuizmo::IsUsing()) {
            vec3 rot;
            mat4::decompose(transform, transformComponent.position, rot, transformComponent.scale);
            transformComponent.rotation = transformComponent.rotation + (rot - transformComponent.rotation); // + deltaRotation, fighting with GimbleLock
            return true;
        }

        return false;
    }


}
