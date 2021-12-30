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


#include "ViewportImGui.h"
#include "InspectorImGui.h"
#include "../../../Logging/Logger.h"
#include "../../public/ServiceLocatorEditor.h"
#include "../../../Window/Window.h"
#include "../../../Core/graphics/public/RenderManager.h"
#include "../../../Core/graphics/public/Framebuffer.h"
#include "../../../Core/ecs/SceneManagerEditor.h"
#include "../../../Core/ecs/Scene.h"
#include "../../../Core/ecs/Entity/EventsCameraEntity.h"
#include "../../../Core/ecs/Entity/EventsComponentEntity.h"


namespace marengine {

    static void drawGuizmo(ImGuizmo::OPERATION guizmoOperation, const FRenderCamera* pRenderCamera,
                           const Entity& currentEntity);


    void FViewportWidgetImGui::create(FServiceLocatorEditor* pServiceLocator) {
        m_pRenderManager =
                pServiceLocator->retrieve<FHolderPtr<FRenderManager*>>()->pInstance;
        m_pSceneManagerEditor =
                pServiceLocator->retrieve<FHolderPtr<FSceneManagerEditor*>>()->pInstance;
        m_pInspectorWidget =
                pServiceLocator->retrieve<FInspectorWidgetImGui>();
        m_pWindow =
                pServiceLocator->retrieve<FHolderPtr<FWindow*>>()->pInstance;

        m_camera.create(m_pWindow, m_pRenderManager->getViewportFramebuffer());

        if(m_pSceneManagerEditor->usingEditorCamera()) {
            FEventsCameraEntity::onEditorCameraSet(m_camera.getRenderCamera());
        }
    }

    void FViewportWidgetImGui::updateFrame() {
        const ImGuizmo::OPERATION guizmoOperation{ displayViewportControlPanel() };

        ImGuiStyle& style{ ImGui::GetStyle() };
        const ImVec2 rememberDefaultVal{ style.WindowPadding };
        style.WindowPadding = ImVec2(1.f, 1.f);

        ImGui::Begin("Viewport", nullptr);

        displayActualViewport();

        const bool userInEditorMode =
            m_pSceneManagerEditor->usingEditorCamera() && m_pSceneManagerEditor->isEditorMode();

        if (userInEditorMode) {
            if (m_pInspectorWidget->isInspectedEntityValid()) {
                drawGuizmo(guizmoOperation, m_camera.getRenderCamera(),
                           m_pInspectorWidget->getInspectedEntity());
            }

            bool cameraUpdated{ m_camera.update() };

            if(ImGui::IsWindowFocused()) {
                if(FCameraKeyboardDecorator::update(&m_camera, MAR_MOUSE_BUTTON_RIGHT)) {
                    cameraUpdated = true;
                }
                if(FCameraMouseDecorator::update(&m_camera, MAR_MOUSE_BUTTON_RIGHT)) {
                    cameraUpdated = true;
                }
                if(FCameraSphericalDecorator::update(&m_camera, MAR_MOUSE_BUTTON_MIDDLE)) {
                    cameraUpdated = true;
                }
            }

            if (cameraUpdated) {
                m_camera.recalculate();
                FEventsCameraEntity::onEditorCameraSet(m_camera.getRenderCamera());
            }
        }

        ImGui::End();

        style.WindowPadding = rememberDefaultVal;
    }

    static void displayEditorModeButtons(FSceneManagerEditor* pSceneManagerEditor,
                                         const FRenderCamera* pRenderCamera) {
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
                                       const FRenderCamera* pRenderCamera) {
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
        static int32 guizmoIndex{ GuizmoType::NONE };
        constexpr std::array<const char*, guizmoTypeCount> guizmoTypes{
            "None", "Translation", "Rotation", "Scale"
        };
        constexpr uint8 renderingModeCount{ 2 };
        enum RenderingType { TRIANGLES, LINES };
        static int32 renderingIndex{ RenderingType::TRIANGLES };
        constexpr std::array<const char*, renderingModeCount> renderingModes{
            "Triangles", "Lines"
        };

        ImGui::Begin("Viewport Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            ImGui::Columns(4);
            ImGui::SetColumnWidth(0, 200.f);

            if (m_pSceneManagerEditor->isEditorMode()) {
                displayEditorModeButtons(m_pSceneManagerEditor, m_camera.getRenderCamera());
            }
            else {
                displayPlayModeButtons(m_pSceneManagerEditor, m_camera.getRenderCamera());
            }

            ImGui::NextColumn();
            ImGui::Combo("RenderMode", &renderingIndex, renderingModes.data(), renderingModeCount);
            ImGui::NextColumn();
            ImGui::Combo("GuizmoType", &guizmoIndex, guizmoTypes.data(), guizmoTypeCount);
            ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::EndMenuBar();
        }

        ImGui::End();

        if (m_pWindow->isKeyPressed(MAR_KEY_LEFT_CONTROL)) {
            if (m_pWindow->isKeyPressed(MAR_KEY_Z)) { guizmoIndex = GuizmoType::TRANSLATION; }
            if (m_pWindow->isKeyPressed(MAR_KEY_X)) { guizmoIndex = GuizmoType::ROTATION; }
            if (m_pWindow->isKeyPressed(MAR_KEY_C)) { guizmoIndex = GuizmoType::SCALE; }
            if (m_pWindow->isKeyPressed(MAR_KEY_V)) { guizmoIndex = GuizmoType::NONE; }
        }

        switch(renderingIndex) {
            case RenderingType::LINES: FRenderMode::setLines(); break;
            default: FRenderMode::setTriangles(); break;
        }

        switch(guizmoIndex) {
            case GuizmoType::TRANSLATION: return ImGuizmo::OPERATION::TRANSLATE;
            case GuizmoType::ROTATION: return ImGuizmo::OPERATION::ROTATE;
            case GuizmoType::SCALE: return ImGuizmo::OPERATION::SCALE;
            default: return ImGuizmo::OPERATION::NONE;
        }
    }

    void FViewportWidgetImGui::displayActualViewport() {
        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        FFramebuffer* pFramebuffer{ m_pRenderManager->getViewportFramebuffer() };
        const bool needToResize =
                pFramebuffer->getWidth() != (uint32)viewportSize.x
                || pFramebuffer->getHeight() != (uint32)viewportSize.y;
        if(needToResize) {
            pFramebuffer->resize((uint32)viewportSize.x, (uint32)viewportSize.y);
        }

        const uint32_t id{ pFramebuffer->getColorAttach() };
        ImGui::Image((ImTextureID)id, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }


    static bool draw(ImGuizmo::OPERATION guizmoOperation, const FRenderCamera* pRenderCamera,
                     CTransform& transformComponent);

    void drawGuizmo(ImGuizmo::OPERATION guizmoOperation, const FRenderCamera* pRenderCamera,
                    const Entity& currentEntity) {
        auto& transform = currentEntity.getComponent<CTransform>();
        if (guizmoOperation == ImGuizmo::OPERATION::NONE) {
            return;
        }

        const bool userUsedGuizmo{ draw(guizmoOperation, pRenderCamera, transform) };
        if (userUsedGuizmo) {
            FEventsComponentEntity::onUpdate<CTransform>(currentEntity);
        }
    }

    bool draw(ImGuizmo::OPERATION guizmoOperation, const FRenderCamera* pRenderCamera,
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
            pView = pRenderCamera->getView().value_ptr();
            pProjection = pRenderCamera->getProjection().value_ptr();
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
